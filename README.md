# ue5_boxtest

  # UE5 FPS Box Game

## Project Overview
This is a first-person shooter game in Unreal Engine 5.6 where the player spawns and shoots interactive boxes. Boxes are dynamically generated based on JSON data fetched from an online source. Each box has properties such as color, health, and score. Players earn points for destroying boxes, which are reflected in real-time on a UI score widget.

## Class & Logic Overview
- **UWebManager**: Fetches JSON from the online URL using UE native HTTP. Once fetched, it broadcasts the data via a delegate (`OnJSONReady`) to notify other classes.
- **UJSONParser**: Parses the JSON to extract box types and instances:
  - **BoxTypes**: Name, Color, Health, Score
  - **BoxInstances**: Location, Rotation, Scale
  - Stores parsed data in arrays for spawning.
- **ABoxSpawner**: Spawns boxes in the world using parsed JSON data:
  - Assigns dynamic materials based on JSON color.
  - Tracks health and score for each spawned box.
  - `DamageBox()` handles hits, reduces health, destroys boxes at zero health, and returns points.
  - Helpers like `FindBoxType()` and `CreateBoxMaterial()` simplify material assignment and mapping.
- **Aue5_fpsboxCharacter**: Handles player controls:
  - Movement, jumping, aiming.
  - **Fire weapon**: Left Mouse Button (`FireWeaponAction`)
  - **Spawn box**: Key F (`SpawnBoxAction`)
  - Shooting uses line traces to interact with boxes.
  - Updates player score via `UScoreWidget`.
- **UScoreWidget**: Displays the player's score and updates dynamically with `UpdateScore()`.

## Challenges
- Implementing **native C++ JSON fetching and parsing** without external libraries.
- Assigning **dynamic materials** to each spawned box using JSON color data.
- Managing **box health, destruction, and score tracking** efficiently.
- Coordinating multiple systems (WebManager → JSONParser → BoxSpawner → Character → UI) cleanly.
- Maintaining **UE C++ conventions and feature-based folder structure**.

## How to Test
1. Open the project in **Unreal Engine 5.6**.
2. Press **Play**.
3. **Spawn boxes** by pressing **F**.
4. **Shoot boxes** with the **Left Mouse Button**.
5. Verify:
   - Boxes take damage and are destroyed when health reaches zero.
   - Player score updates correctly on the UI.
   - Console logs show JSON fetching, parsing, box spawning, and material application.
6. Optional: Check debug logs for detailed spawn and damage information.


