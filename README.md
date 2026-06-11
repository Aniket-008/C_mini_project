# 2D Graphics Editor

This project is a simple text-based 2D graphics editor built in C. It uses a 2D array of characters to store the picture and draws shapes using `*` over an `_` background.

## Features

- Draw rectangle, line, triangle, and circle shapes
- Add shapes to the picture
- Delete shapes by ID
- Modify existing shapes
- Display the canvas as a 2D grid
- List the current shapes in the picture

## Build

Use a C compiler such as `gcc`.

```sh
gcc main.c -o graphics_editor
```

## Run

```shn
./graphics_editor
```

## Notes

- Canvas size is 80x25 characters.
- Coordinates are entered as `x y` pairs.
- The program uses `*` for shape pixels and `_` for empty canvas pixels.
