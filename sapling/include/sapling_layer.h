#pragma once

#include "Sapfire.h"
#include "core/core.h"

namespace widgets {
	class SSceneHierarchy;
}

class SSubeditor;

class SaplingLayer final : public Sapfire::Layer {
public:
	SaplingLayer();
	~SaplingLayer() final = default;
	void on_attach() final;
	void on_detach() final;
	void on_update(Sapfire::f32 delta_time) final;
	void on_event(Sapfire::Event& e) final;
	void on_render() final;

private:
	void update_pass_cb(Sapfire::f32 delta_time);
	bool on_window_resize_finished(Sapfire::WindowResizeFinishedEvent&);
	bool on_window_resize(Sapfire::WindowResizeEvent& e);

private:
	Sapfire::stl::vector<Sapfire::stl::unique_ptr<SSubeditor>> m_Subeditors{};
	Sapfire::stl::unique_ptr<Sapfire::d3d::GraphicsDevice> m_GraphicsDevice{};
    Sapfire::d3d::PipelineState m_PipelineState{};
	Sapfire::d3d::Texture m_DepthTexture{};
};