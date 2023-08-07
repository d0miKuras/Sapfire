const std = @import("std");
const zgui = @import("zgui");
const log = @import("logger.zig");
const crypto = std.crypto;
const json = std.json;
const jobs = @import("jobs.zig");
const nfd = @import("nfd");

const sf = struct {
    usingnamespace @import("../renderer/texture.zig");
    usingnamespace @import("../renderer/mesh.zig");
    usingnamespace @import("../renderer/material.zig");
    usingnamespace @import("../scene/scene.zig");
};

// TODO: add SceneAssetManager.
// The purpose is to separate runtime and edittime asset managers.
// Editor asset manager will load ALL assets to memory, while runtime asset manager will only load those used in the scene.
pub const AssetManager = struct {
    allocator: std.mem.Allocator,
    texture_manager: sf.TextureManager,
    material_manager: sf.MaterialManager,
    mesh_manager: sf.MeshManager,
    scene_manager: sf.SceneManager,

    pub fn texture_manager() *sf.TextureManager {
        return &instance.texture_manager;
    }

    pub fn material_manager() *sf.MaterialManager {
        return &instance.material_manager;
    }

    pub fn mesh_manager() *sf.MeshManager {
        return &instance.mesh_manager;
    }

    pub fn scene_manager() *sf.SceneManager {
        return &instance.scene_manager();
    }

    // TODO: takes in path to "project" serialization which contains paths to each subsystem's serialization file.
    // Each submodule will have its own file so that project's assets can be loaded in parallel
    pub fn init(
        allocator: std.mem.Allocator,
        project_config: []const u8,
    ) !void {
        instance.allocator = allocator;
        var arena = std.heap.ArenaAllocator.init(allocator);
        defer arena.deinit();
        const config_data = std.fs.cwd().readFileAlloc(arena.allocator(), project_config, 512 * 16) catch |e| {
            log.err("Failed to parse project config file.", .{});
            return e;
        };
        const Config = struct {
            texture_config: []const u8,
            material_config: []const u8,
            mesh_config: []const u8,
            scene_config: []const u8,
        };
        const config = try json.parseFromSliceLeaky(Config, arena.allocator(), config_data, .{});

        _ = try jobs.JobsManager.jobs().schedule(jobs.JobId.none, struct {
            allocator: std.mem.Allocator,
            path: []const u8,
            texman: *sf.TextureManager,
            pub fn exec(self: *@This()) void {
                self.texman.* = sf.TextureManager.init(self.allocator, self.path) catch {
                    return;
                };
            }
        }{
            .allocator = allocator,
            .path = config.texture_config,
            .texman = &instance.texture_manager,
        });
        jobs.JobsManager.jobs().start();
        const material_job = try jobs.JobsManager.jobs().schedule(jobs.JobId.none, struct {
            allocator: std.mem.Allocator,
            path: []const u8,
            matman: *sf.MaterialManager,
            pub fn exec(self: *@This()) void {
                self.matman.* = sf.MaterialManager.init(self.allocator, self.path) catch {
                    return;
                };
            }
        }{
            .allocator = allocator,
            .path = config.material_config,
            .matman = &instance.material_manager,
        });

        _ = try jobs.JobsManager.jobs().schedule(material_job, struct {
            allocator: std.mem.Allocator,
            path: []const u8,
            meshman: *sf.MeshManager,
            pub fn exec(self: *@This()) void {
                self.meshman.* = sf.MeshManager.init(self.allocator, self.path) catch {
                    return;
                };
            }
        }{
            .allocator = allocator,
            .path = config.mesh_config,
            .meshman = &instance.mesh_manager,
        });
        _ = try jobs.JobsManager.jobs().schedule(jobs.JobId.none, struct {
            pub fn exec(_: *@This()) void {
                jobs.JobsManager.jobs().stop();
            }
        }{});
        jobs.JobsManager.jobs().join();
        instance.scene_manager = try sf.SceneManager.init(allocator, config.scene_config);
    }

    pub fn deinit() void {
        sf.MaterialManager.deinit(&instance.material_manager);
        sf.MeshManager.deinit(&instance.mesh_manager);
        sf.TextureManager.deinit(&instance.texture_manager);
        sf.SceneManager.deinit(&instance.scene_manager);
    }

    // TODO: this should be used to import raw files and generate .sf* format assets
    pub fn create_asset(
        path: []const u8,
        asset_type: AssetType,
    ) void {
        _ = asset_type;
        var guid: [64]u8 = undefined;
        crypto.hash.sha2.Sha512.hash(path, guid[0..], .{});
    }

    pub fn generate_guid(path: [:0]const u8) [64]u8 {
        var guid: [64]u8 = undefined;
        crypto.hash.sha2.Sha512.hash(path, &guid, .{});
        return guid;
    }

    pub fn draw_explorer() void {
        if (zgui.begin("Asset explorer", .{})) {
            if (zgui.button("Add", .{})) {}
        }
        zgui.end();
    }
};

pub const AssetType = enum {
    Texture,
    Material,
    Mesh,
    Scene,
};

var instance: AssetManager = undefined;
pub fn asset_manager() *AssetManager {
    return &instance;
}
