# The Machinery Icon Font

https://github.com/OurMachinery/ionicons

This project holds the icon font used by The Machinery to draw icons. It is installed in the
`data/fonts` directory when The Machinery is built.

Most of the icons in this front are from [Ionicons](https://ionicons.com/) v5.0.1. Ionicons is
licensed under MIT. The
[ionicons.json](https://github.com/OurMachinery/ionicons/blob/main/ionicons.json) glyph map and TTF
file are from https://github.com/oblador/react-native-vector-icons. This project is also licensed
under MIT.

In addition, this project also contains some custom icons in the
[the-machinery-add-ons](https://github.com/OurMachinery/ionicons/tree/main/the-machinery-add-ons)
folder with a glyph map in
[the-machinery-add-ons.json](https://github.com/OurMachinery/ionicons/blob/main/the-machinery-add-ons.json).

This project is licensed under the MIT license.

## Adding Icons

The Ionicons icons are stored in the private use area `0xF100`. The Machinery icons are stored
in the private use area `0xE000`.

To add new icons to the font, use the following steps:

1. Draw the icons in a vector draw program. The icons should be drawn in a 512 x 512 px area and
   use a 36 px stroke width.

2. Convert strokes into shapes and unionize any intersecting/overlapping shapes. In Affinity
   Designer, this can be done with the *Expand Stroke* and *Add* operations.

3. Export the icon files as `.svg` files in the `the-machinery-add-ons` folder. Give them suitable
   names.

4. Use [FontForge](https://fontforge.org/en-US/) to import the `.svg` files into `ionicons.ttf`.
   Add the icons at the next free spot in the `0xE000` private use area. Use the *Import* command to
   import the `.svg` files.

5. Use *File > Generate Fonts...* in FontForge to re-generate the `.ttf` file.

6. Update `the-machinery-add-ons.json` with the name and glyph index of the new icons you add.

7. Run

   ~~~
   zig cc generate_header.c -I%TM_SDK_DIR%/headers -DTM_LINKS_FOUNDATION  %TM_SDK_DIR%/lib/vs2019/Debug/foundation.lib && a.out
   ~~~

   to re-generate the `ionicons.h` file from the `.json` files. (Or manually update the `ionicons.h`
   file.)

8. Generate and upload a new version of the ionicions library to our library server.