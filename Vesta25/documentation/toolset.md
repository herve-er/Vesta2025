# AI Assistant - Toolset Management

## Overview
This document describes the toolset management system for the AI assistant. The assistant can dynamically load and unload toolsets to optimize performance and reduce token usage.

## Toolset Structure
Toolsets are organized hierarchically to group related functionalities efficiently. Each tool belongs to a toolset and follows a specific path notation:

```
home_automation/entertainment/media/video
```

### Example Structure
- **home_automation**
  - **entertainment**
    - **media**
      - **video**
        - `getVideoReceiverList`
        - `setInput`
      - **audio**
        - `getSpeakerList`
        - `setVolume`
    - **lighting**
      - `turnOnLights`
      - `adjustBrightness`

## Tool Loading and Unloading
To access a tool, the assistant must first load the corresponding toolset. It can load multiple toolsets simultaneously. Once finished, the toolset should be unloaded to free resources.

### Loading a Toolset
Before using any tool in a toolset, the assistant must load the toolset by specifying its path:
```json
{
  "action": "load_toolset",
  "toolset": "home_automation/entertainment/media/video"
}
```

### Accessing a Tool
Once the toolset is loaded, tools within it can be used:
```json
{
  "action": "use_tool",
  "tool": "getVideoReceiverList",
  "toolset": "home_automation/entertainment/media/video"
}
```

### Unloading a Toolset
After completing operations, the assistant should unload the toolset to optimize resource usage:
```json
{
  "action": "unload_toolset",
  "toolset": "home_automation/entertainment/media/video"
}
```

## Tool Descriptions
Each tool description must include the toolset path for clarity.

**Example Tool Description:**
```json
{
  "name": "getVideoReceiverList",
  "description": "Retrieves the list of available video receivers.",
  "toolset": "home_automation/entertainment/media/video"
}
```

## Summary
- Toolsets are organized hierarchically.
- The assistant must load a toolset before using its tools.
- Multiple toolsets can be loaded simultaneously.
- Toolsets should be unloaded when no longer needed.
- Each tool description must include its toolset path.
