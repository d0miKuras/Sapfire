struct CameraUniform {
    view_proj: mat4x4<f32>
};

@group(0) @binding(0)
var<uniform> camera: CameraUniform;

struct LightUniform {
    position: vec3<f32>,
    color: vec3<f32>
};

@group(1) @binding(0)
var<uniform> light: LightUniform;

struct VertexInput {
    @location(0) position: vec3<f32>
};

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) color: vec3<f32>
}

@vertex
fn vs_main(model: VertexInput) -> VertexOutput {
    let scale = 0.25;
    var out: VertexOutput;
    out.clip_position = camera.view_proj * vec4<f32>(model.position * scale + light.position, 1.0);
    out.color = light.color;
    return out;
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(input.color, 1.0);
}