mod opengl_shader;
use crate::renderer::Renderer;
pub use opengl_shader::*;

pub struct OpenGLRenderContext {
    pub shader: OpenGLShader,
}

impl Renderer for OpenGLRenderContext {
    fn add_shader(&mut self, vertex_path: &str, frag_path: &str) {
        self.shader = OpenGLShader::new(vertex_path, frag_path);
    }
}
