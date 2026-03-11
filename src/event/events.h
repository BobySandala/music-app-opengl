#pragma once

struct PlayEvent {};
struct StopEvent {};
struct PauseEvent{};
struct ResumeEvent{};
struct TogglePlayEvent{};
struct ExitEvent {};

// Mouse events
struct MouseClickEvent
{
    int button; // GLFW mouse button
    double x {}; // Cursor position x
    double y {}; // Cursor position y
};