set(RELATIVE_SAPFIRE_TRANSLATION_UNITS
        engine/include/core/entry.h
        engine/src/core/logger.cpp
        engine/src/core/application.cpp
        engine/src/core/layer_stack.cpp
        engine/src/core/input.cpp
        engine/src/core/file_system.cpp
        engine/src/core/game_context.cpp
        engine/src/core/uuid.cpp
        engine/src/core/rtti.cpp
        engine/src/core/memory.cpp
        engine/src/assets/mesh_manager.cpp
        engine/src/assets/texture_manager.cpp
        engine/src/assets/asset_manager.cpp
        engine/src/components/components.cpp
        engine/src/components/transform.cpp
        engine/src/components/name_component.cpp
        engine/src/components/entity.cpp
        engine/src/components/ec_manager.cpp
        engine/src/components/test_custom_component.cpp
        engine/src/components/movement_component.cpp
        engine/src/components/render_component.cpp
        engine/src/tools/shader_compiler.cpp
        engine/src/tools/texture_loader.cpp
        engine/src/tools/obj_loader.cpp
        engine/src/render/camera.cpp
        engine/src/render/command_queue.cpp
        engine/src/render/compute_context.cpp
        engine/src/render/context.cpp
        engine/src/render/copy_context.cpp
        engine/src/render/d3d_primitives.cpp
        engine/src/render/d3d_util.cpp
        engine/src/render/descriptor_heap.cpp
        engine/src/render/resources.cpp
        engine/src/render/graphics_context.cpp
        engine/src/render/graphics_device.cpp
        engine/src/render/memory_allocator.cpp
        engine/src/render/pipeline_state.cpp
        engine/src/physics/physics_engine.cpp
        engine/src/render/window.cpp
)

foreach(TU ${RELATIVE_SAPFIRE_TRANSLATION_UNITS})
    set(TU ${CMAKE_CURRENT_SOURCE_DIR}/${TU})
    list(APPEND SAPFIRE_TRANSLATION_UNITS ${TU})
endforeach(TU)