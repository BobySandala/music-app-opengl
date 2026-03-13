#include "audio/audio_engine.h"
#include "core/window.h"
#include "core/input.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "event/event_bus.h"
#include "event/events.h"
#include "graphics/renderer.h"
#include "graphics/ui_system/ui_element.h"

void busSubsribe(Window& window, AudioEngine& audioEngine, EventBus& bus)
{
    // Subscribe to play event
    bus.subscribe<PlayEvent>([&audioEngine](const PlayEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Play, ""};
        audioEngine.enqueue(cmd);
    });

    // Subscribe to stop event
    bus.subscribe<StopEvent>([&audioEngine](const StopEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Stop, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to pause event
    bus.subscribe<PauseEvent>([&audioEngine](const PauseEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Pause, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to resume event
    bus.subscribe<ResumeEvent>([&audioEngine](const ResumeEvent)
    {
        std::cout << "Resume event received\n";
        AudioCommand cmd {AudiocommandType::Resume, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to toggle play event
    bus.subscribe<TogglePlayEvent>([&audioEngine](const TogglePlayEvent)
    {
        std::cout << "Toggle Play Event Received\n";
        AudioCommand cmd {AudiocommandType::TogglePlay, ""};
        audioEngine.enqueue(cmd);
    });

    // Subscribe to exit event
    bus.subscribe<ExitEvent>([&window, &audioEngine](const ExitEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Exit, ""};
        audioEngine.enqueue(cmd);
        window.shutdown();
    });

    // Subscribe to mouse click event
    bus.subscribe<MouseClickEvent>([](const MouseClickEvent& e){
        std::cout << "Mouse click: button=" << e.button << " x=" << e.x << " y=" << e.y << "\n";
    });
}

int main()
{
    Window window(800, 600, "Music Player");

    if(!window.init())
        return -1;

    AudioEngine audioEngine;
    EventBus bus;
    Renderer renderer;
    renderer.init();

    UIElement button(0.0f, 0.0f, 0.3f, 0.2f, 1.0f, 0.0f, 0.0f);
    // Input system
    Input input(bus);
    input.init(window.getNativeWindow());

    //busSubsribe(window, audioEngine, bus);
    while(!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawQuad(button.getQuad());

        window.update();

        // Process queued events
        bus.process();
    }
    window.shutdown();
}