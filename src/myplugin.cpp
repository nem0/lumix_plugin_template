#define LUMIX_NO_CUSTOM_CRT
#include "engine/engine.h"
#include "engine/plugin.h"
#include "engine/stream.h"
#include "engine/world.h"
#include "imgui/imgui.h"


using namespace Lumix;


// each world has its own instance of this scene
struct PluginScene : IScene {
	PluginScene(Engine& engine, IPlugin& plugin, World& world, IAllocator& allocator)
		: m_engine(engine)
		, m_plugin(plugin)
		, m_world(world)
		, m_allocator(allocator)
	{}

	void serialize(struct OutputMemoryStream& serializer) override {
		// save our scene data
		serializer.write(m_some_value);
	}

	void deserialize(struct InputMemoryStream& serializer, const struct EntityMap& entity_map, i32 version) override {
		// load our scene data
		serializer.read(m_some_value);

	}
	IPlugin& getPlugin() const override { return m_plugin; }
	World& getWorld() override { return m_world; }
	
	void update(float time_delta, bool paused) {
		// called each frame
		if (!paused) m_some_value += time_delta; 
	}
	
	void clear() override {
		// called before scene is destroyed, clean up your data
	}

	Engine& m_engine;
	IPlugin& m_plugin;
	World& m_world;
	IAllocator& m_allocator;
	float m_some_value = 0;
};


// there will be only one instance of system
struct PluginSystem : IPlugin {
	PluginSystem(Engine& engine) : m_engine(engine) {}

	u32 getVersion() const override { return 0; }
	const char* getName() const override { return "myplugin"; }
	
	void serialize(OutputMemoryStream& serializer) const override {}
	bool deserialize(u32 version, InputMemoryStream& serializer) override { return true; }

	void createScenes(World& world) override {
		// this is when a world is created
		// usually we want to add our scene to world here
		IAllocator& allocator = m_engine.getAllocator();
		UniquePtr<PluginScene> scene = UniquePtr<PluginScene>::create(allocator, m_engine, *this, world, allocator);
		world.addScene(scene.move());
	}

	Engine& m_engine;
};


LUMIX_PLUGIN_ENTRY(myplugin)
{
	return LUMIX_NEW(engine.getAllocator(), PluginSystem)(engine);
}


