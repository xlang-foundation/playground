This Win32 application features a simple graphical user interface and includes two key API functions for managing drawable objects within a window:

1. **`DrawObj` Function**: 
   - Purpose: Adds a new drawable object to the application's window.
   - Parameters: The function accepts the `x` and `y` coordinates for the object's position, the `width (w)` and `height (h)` of the object, and the `objName` which is a string label for the object.
   - Functionality: When invoked, it creates a new drawable object and assigns it a unique ID. The object is then stored in a map (an STL container) for easy retrieval and management. Each object is essentially a text label drawn at the specified coordinates.
   - Returns: A unique integer ID assigned to the newly created object.

2. **`UpdateObj` Function**:
   - Purpose: Updates the position of an existing drawable object within the window.
   - Parameters: It takes an `id` (which identifies the drawable object), and new `x` and `y` coordinates for the updated position.
   - Functionality: This function locates the object by its ID in the map and updates its position with the new coordinates provided. If the object with the given ID exists, its position is updated; otherwise, no action is taken.

These functions interact with a standard Win32 application structure, utilizing a message loop and window procedure (`WndProc`) to handle events like painting the window (`WM_PAINT`). During the `WM_PAINT` event, all drawable objects stored in the map are rendered in their specified positions within the application's window, allowing for real-time visual updates when objects are added or their positions are changed.