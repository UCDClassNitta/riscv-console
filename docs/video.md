# Video Controller Model 02
The model 02 video controller replaced the original [model 01 video controller](./video01.md), and is now responsible for rendering the graphics for the console. The video controller has 1MiB of memory and renders a 512 x 288 (16:9 aspect ratio) screen. The video controller has a [text mode](#text-mode) and a [graphics mode](#graphics-mode). 

## Memory Map
The video controller has 1MiB of memory that is used for controlling what is displayed on the screen. The table below shows the memory map of the Video Controller.

| Base Address | Size             | Description                                                         |
| ------------ | ---------------- | ------------------------------------------------------------------- |
| 0x50000000   | 0x90000 (576KiB) | [Background Data](#background-data-memory-map)                      |
| 0x50090000   | 0x40000 (256KiB) | [Large Sprite Data](#large-sprite-data-memory-map)                  |
| 0x500D0000   | 0x10000 (64KiB)  | [Medium Sprite Data](#medium-sprite-data-memory-map)                |
| 0x500E0000   | 0x10000 (64KiB)  | [Small Sprite Data](#small-sprite-data-memory-map)                  |
| 0x500F0000   | 0x1000 (4KiB)    | [Background Palettes](#background-and-sprite-palette-memory-map)    |
| 0x500F1000   | 0x1000 (4KiB)    | [Large Sprite Palettes](#background-and-sprite-palette-memory-map)  |
| 0x500F2000   | 0x1000 (4KiB)    | [Medium Sprite Palettes](#background-and-sprite-palette-memory-map) |
| 0x500F3000   | 0x1000 (4KiB)    | [Small Sprite Palettes](#background-and-sprite-palette-memory-map)  |
| 0x500F4000   | 0x800 (2KiB)     | [Font Data](#font-data)                                             |
| 0x500F4800   | 0x900 (2.25KiB)  | [Text Data](#text-data)                                             |
| 0x500F5100   | 0x900 (2.25KiB)  | [Text Color](#text-color)                                           |
| 0x500F5A00   | 0x100 (256B)     | [Background Controls](#background-control-memory-map)               |
| 0x500F5B00   | 0x400 (1KiB)     | [Large Sprite Controls](#large-sprite-controls-memory-map)          |
| 0x500F5F00   | 0x400 (1KiB)     | [Medium Sprite Controls](#medium-sprite-controls-memory-map)        |
| 0x500F6300   | 0x400 (1KiB)     | [Small Sprite Controls](#small-sprite-controls-memory-map)          |
| 0x500F6700   | 0x80 (128B)      | [Text Palettes](#text-palette)                                      |
| 0x500F6780   | 0x4 (4B)         | [Mode Control Register](#mode-control)                              |

## Text Mode
The default of the video controller is to start in text mode that provides 64 x 36 characters each of 8 x 8 pixels. The MSX font is loaded into the font memory at reset. The text mode is capable of rendering up to 256 different characters, but by default the MSX font only supports the printable ASCII characters from `'!'` to `'~'`. The color data is zeroed out and the color palette is set to default values at reset.

#### Font Data
Font data is organized as 8-byte bit masks for each of the 256 characters. The order of the bytes is from top row to bottom row, with the order of bits being from left to right with left being most significant bit. An example of the bitmask can be seen below:

![](img/bitmap-1.png)

#### Text Data
The text data is ordered from top row down to the bottom row and from left to right. Address 0 offset for the text data is the upper left corner.

#### Text Color
The text color data is ordered from top row down to the bottom row and from left to right. Address 0 offset for the text color data is the upper left corner. Below shows the structure of the color entries, both the background and foreground colors are indexed in one entry.
| Bit         | 7..4     | 3..0     |
| ----------- | -------- | -------- |
| Description | BG Color | FG Color |

#### Text Palette
The text mode has two 16 entry palettes, one for the background and one for the foreground colors. Below is the layout for text mode palettes. Each entry is of the same form of other the [palette entries](#palette-entry).
| Base Address | Size         | Description                 |
| ------------ | ------------ | --------------------------- |
| 0x500F6700   | 0x4(4B)      | Foreground Palette Entry 0  |
| 0x500F6704   | 0x4(4B)      | Foreground Palette Entry 1  |
| ...          | ...          | ...                         |
| 0x500F673C   | 0x4(4B)      | Foreground Palette Entry 15 |
| 0x500F6740   | 0x4(4B)      | Background Palette Entry 0  |
| ...          | ...          | ...                         |
| 0x500F6778   | 0x4(4B)      | Background Palette Entry 14 |
| 0x500F677C   | 0x4(4B)      | Background Palette Entry 15 |

## Graphics Mode
The graphics mode provides support for four fully unique ["background" full resolution images](#background-images) (or up to 64 tiled backgrounds), 256, 64 x 64 pixel [large sprites](#large-sprites) (with 64 unique), 256, 32 x 32 pixel [medium sprites](#medium-sprites) (with 64 unique), and 256, 16 x 16 pixel [small sprites](#small-sprites). All images support 256 unique colors from a 32-bit RGBA [palette](#palette). There are four [background palettes](#background-and-sprite-palette-memory-map) and four [sprite palettes](#background-and-sprite-palette-memory-map) for each sprite size.

### Background Images
The "background" images can be interpreted in one of two ways depending upon the control structure. In "pixel" mode the background data section provides four full 512 x 288 pixel resolution images. Each "background" image is composed of 147,456B (512 x 288 1B values) pixel values. In "tile" mode the background data section can be composed of up to eight 16 x 16 pixel tile palettes, with up to eight 512 x 288 images composed of 576 (32 x 18) 16 x 16 pixel tiles for each tile palette. It should be noted that the "pixel" and "tile" mdoes can be used at the same time, for example the background data for the image 0 could be used in pixel mode, while image 1 could be utilized as tile mode images 2 & 3. There are 64, 32-bit control structures for the background images, unlike the [model 01 video controller](./video01.md) the controls are not mapped one-to-one with data sections. The control structure controls the X, Y and Z position of the image, as well as a selector for the palette to use, the mode ("pixel" or "tile"), and the index into the data section. The 10-bit X and Y offsets control the upper left corner position of the image. The 3-bit Z position specifies the Z plane in which the image will be rendered. Images are rendered in increasing Z position; within a Z position, images are rendered in order of their control structure number. 

#### Background Data Memory Map
Below is the memory map layout for the background data images in "pixel" mode.
| Base Address | Size             | Description                         |
| ------------ | ---------------- | ----------------------------------- |
| 0x50000000   | 0x24000 (144KiB) | Background Data Image Pixel Mode 0  |
| 0x50024000   | 0x24000 (144KiB) | Background Data Image Pixel Mode 1  |
| 0x50048000   | 0x24000 (144KiB) | Background Data Image Pixel Mode 2  |
| 0x5006C000   | 0x24000 (144KiB) | Background Data Image Pixel Mode 3  |

Below is the memory map layout for the background data images in "tile" mode.
| Base Address | Size             | Description                         |
| ------------ | ---------------- | ----------------------------------- |
| 0x50000000   | 0x12000 (72KiB) | Background Data Image Tile Mode 0    |
| 0x50012000   | 0x12000 (72KiB) | Background Data Image Tile Mode 1    |
| 0x50024000   | 0x12000 (72KiB) | Background Data Image Tile Mode 2    |
| 0x50036000   | 0x12000 (72KiB) | Background Data Image Tile Mode 3    |
| 0x50048000   | 0x12000 (72KiB) | Background Data Image Tile Mode 4    |
| 0x5005A000   | 0x12000 (72KiB) | Background Data Image Tile Mode 5    |
| 0x5006C000   | 0x12000 (72KiB) | Background Data Image Tile Mode 6    |
| 0x5007E000   | 0x12000 (72KiB) | Background Data Image Tile Mode 7    |

Below is the memory map layout for the Background Data Images Tile Mode 0 data.
| Base Address | Size          | Description            |
| ------------ | ------------- | ---------------------- |
| 0x50000000   | 0x100 (256B)  | Tile Image Entry 0     |
| 0x50000100   | 0x100 (256B)  | Tile Image Entry 1     |
| ...          | ...           | ...                    |
| 0x5000FE00   | 0x100 (256B)  | Tile Image Entry 254   |
| 0x5000FF00   | 0x100 (256B)  | Tile Image Entry 255   |
| 0x50010000   | 0x240 (576B)  | Sub Image Tile Data 0  |
| 0x50010240   | 0x240 (576B)  | Sub Image Tile Data 1  |
| ...          | ...           | ...                    |
| 0x50010D80   | 0x240 (576B)  | Sub Image Tile Data 6  |
| 0x50010FC0   | 0x240 (576B)  | Sub Image Tile Data 7  |
| 0x50011200   | 0xE00 (3KiB)  | Unused                 |

#### Background Control Memory Map
Below is the memory map layout for the background controls.
| Base Address | Size    | Description              |
| ------------ |-------- | ------------------------ |
| 0x500F5100   | 0x4(4B) | Background Controls 0    |
| 0x500F5104   | 0x4(4B) | Background Controls 1    |
| ...          | ...     | ...                      |
| 0x500F51F8   | 0x4(4B) | Background Controls 62   |
| 0x500F51FC   | 0x4(4B) | Background Controls 63   |

#### Background Control Structure
Below is the structure of the background controls in "pixel" mode.
| Bit         | 31       | 30..29 | 28..25   | 24..22 | 21..12 | 11..2 | 1..0    |
| ----------- | -------- | -------| -------- | ------ |------- | ----- | ------- |
| Description | Mode (0) | Px Idx | Reserved | Z      | Y+288  | X+512 | Palette |

Below is the structure of the background controls in "tile" mode.
| Bit         | 31       | 30..28 | 27..25  | 24..22 | 21..12 | 11..2 | 1..0    |
| ----------- | -------- | -------| ------- | ------ |------- | ----- | ------- |
| Description | Mode (1) | Ti Idx | Sub Idx | Z      | Y+288  | X+512 | Palette |


Notice that the X and Y offsets are the interpreted as X+512 and Y+288. This means that the background image width and height are subtracted from the X and Y values respectively. A value of 0 in the X and/or Y of the control structure will keep the background image off screen. The Px Idx is the index to the pixel data when the mode is set to "pixel" mode.  The Ti Idx is the index to the tile data when the mode is set to "tile" mode, the Sub Idx is the sub image to use for the tile set. 

### Large Sprites
The large sprites are 64 x 64 pixels images (smaller sprites could effectively be rendered by using transparency). Each sprite is composed of 4,096B (64 x 64 1B values) pixel values. Each control structure controls the X, Y, and Z position of the sprite, the index of the sprite data to use, as well as a selector for the palette to use. The 10-bit X and 9-bit Y offsets control the upper left corner position of the spite. The 3-bit Z position specifies the Z plane in which the image will be rendered. Images are rendered in increasing Z position; within a Z position, images are rendered in order of their control structure number.

#### Large Sprite Data Memory Map
Below is the memory map layout for the large sprite data.
| Base Address | Size          | Description          |
| ------------ | ------------- | -------------------- |
| 0x50090000   | 0x1000 (4KiB) | Large Sprite Data 0  |
| 0x50091000   | 0x1000 (4KiB) | Large Sprite Data 1  |
| ...          | ...           | ...                  |
| 0x500CD000   | 0x1000 (4KiB) | Large Sprite Data 62 |
| 0x500CF000   | 0x1000 (4KiB) | Large Sprite Data 63 |

#### Large Sprite Controls Memory Map
Below is the memory map layout for the large sprite controls.
| Base Address | Size    | Description                  |
| ------------ | ------- | ---------------------------- |
| 0x500F5B00   | 0x4(4B) | Large Sprite Controls 0      |
| 0x500F5B04   | 0x4(4B) | Large Sprite Controls 1      |
| ...          | ...     | ...                          |
| 0x500F5EF8   | 0x4(4B) | Large Sprite Controls 254    |
| 0x500F5EFC   | 0x4(4B) | Large Sprite Controls 255    |

#### Large Sprite Control Structure
Below is the structure of the large sprite controls.
|  Bit        | 31..30   | 29..24 | 23..21 | 20..12 | 11..2 | 1..0    |
| ----------- | -------- | ------ | ------ | ------ | ----- | ------- |
| Description | Reserved | Index  | Z      | Y+64   | X+64  | Palette |

Notice that the X and Y offsets are the interpreted as X+64 and Y+64. This means that the large sprite max width and max height are subtracted from the X and Y values respectively. A value of 0 in the X and/or Y of the control structure will keep the large sprite off screen. The Index specifies which Large Sprite Data to use from 0 to 63.

### Medium Sprites
The large sprites are 32 x 32 pixels images (smaller sprites could effectively be rendered by using transparency). Each sprite is composed of 1,024B (32 x 32 1B values) pixel values. Each control structure controls the X, Y, and Z position of the sprite, the index of the sprite data to use, as well as a selector for the palette to use. The 10-bit X and 9-bit Y offsets control the upper left corner position of the spite. The 3-bit Z position specifies the Z plane in which the image will be rendered. Images are rendered in increasing Z position; within a Z position, images are rendered in order of their control structure number.

#### Medium Sprite Data Memory Map
Below is the memory map layout for the medium sprite data.
| Base Address | Size           | Description            |
| ------------ | -------------- | ---------------------- |
| 0x500D0000   | 0x400 (1KiB)   | Medium Sprite Data 0   |
| 0x500D0400   | 0x400 (1KiB)   | Medium Sprite Data 1   |
| ...          | ...            | ...                    |
| 0x500DF800   | 0x400 (1KiB)   | Medium Sprite Data 62  |
| 0x500DFC00   | 0x400 (1KiB)   | Medium Sprite Data 63  |

#### Medium Sprite Controls Memory Map
Below is the memory map layout for the medium sprite controls.
| Base Address | Size    | Description                  |
| ------------ | ------- | ---------------------------- |
| 0x500F5F00   | 0x4(4B) | Medium Sprite Controls 0     |
| 0x500F5F04   | 0x4(4B) | Medium Sprite Controls 1     |
| ...          | ...     | ...                          |
| 0x500F62F8   | 0x4(4B) | Medium Sprite Controls 254   |
| 0x500F62FC   | 0x4(4B) | Medium Sprite Controls 255   |

#### Medium Sprite Control Structure
Below is the structure of the medium sprite controls.
|  Bit        | 31..30   | 29..24 | 23..21 | 20..12 | 11..2 | 1..0    |
| ----------- | -------- | ------ | ------ | ------ | ----- | ------- |
| Description | Reserved | Index  | Z      | Y+32   | X+32  | Palette |

Notice that the X and Y offsets are the interpreted as X+32 and Y+32. This means that the medium sprite max width and max height are subtracted from the X and Y values respectively. A value of 0 in the X and/or Y of the control structure will keep the medium sprite off screen. The Index specifies which Medium Sprite Data to use from 0 to 63.

### Small Sprites
The large sprites are images up to 16 x 16 pixels with a minimum size of 1 x 1. Each sprite is composed of 256B (16 x 16 1B values) pixel values and a 32-bit control structure. The control structure controls the X, Y and Z position of the sprite, the width and height, as well as a selector for the palette to use. The 10-bit X and 9-bit Y offsets control the upper left corner position of the spite. The 3-bit Z position specifies the Z plane in which the sprite will be rendered. Sprites are rendered in increasing Z position; within a Z position, sprites are rendered in order of their sprite number.


#### Small Sprite Data Memory Map
Below is the memory map layout for the small sprite data. 
| Base Address | Size         | Description           |
| ------------ | ------------ | --------------------- |
| 0x500E0000   | 0x100 (256B) | Small Sprite Data 0   |
| 0x500E0100   | 0x100 (256B) | Small Sprite Data 1   |
| ...          | ...          | ...                   |
| 0x500FFE00   | 0x100 (256B) | Small Sprite Data 254 |
| 0x500FFF00   | 0x100 (256B) | Small Sprite Data 255 |


#### Small Sprite Controls Memory Map
Below is the memory map layout for the small sprite controls. 
| Base Address | Size    | Description               |
| ------------ | ------- | ------------------------- |
| 0x500F6300   | 0x4(4B) | Small Sprite Controls 0   |
| 0x500F6304   | 0x4(4B) | Small Sprite Controls 1   |
| ...          | ...     | ...                       |
| 0x500F66F8   | 0x4(4B) | Small Sprite Controls 254 |
| 0x500F66FC   | 0x4(4B) | Small Sprite Controls 255 |


#### Small Sprite Control Structure
Below is the structure of the small sprite controls. 
|  Bit        | 31..24 | 23..21 | 20..12 | 11..2 | 1..0    |
| ----------- | ------ | ------ | ------ | ----- | ------- |
| Description | Index  | Z      | Y+16   | X+16  | Palette |


Notice that the X and Y offsets are the interpreted as X+16 and Y+16. This means that the medium sprite max width and max height are subtracted from the X and Y values respectively. A value of 0 in the X and/or Y of the control structure will keep the medium sprite off screen. The Index specifies which Small Sprite Data to use from 0 to 255.

### Palettes
Each palette is composed of 256, 32-bit RGBA values. Each entry supports 16,777,216 possible colors ignoring transparency. If alpha values are used other than 255 (full opacity) the RGB values must be premultiplied by the alpha value. This means that the maximum R, G, or B value can be is equal to the alpha value. The behavior for non premultiplied values is undefined, but likely will lead to the alpha channel being ignored.

#### Background and Sprite Palette Memory Map
Below is the memory layout for the background and sprite palettes. 
| Base Address | Size         | Description             |
| ------------ | ------------ | ----------------------- |
| 0x500F0000   | 0x400 (1KiB) | Background Palette 0    |
| ...          | ...          | ...                     |
| 0x500F0C00   | 0x400 (1KiB) | Background Palette 3    |
| 0x500F1000   | 0x400 (1KiB) | Large Sprite Palette 0  |
| ...          | ...          | ...                     |
| 0x500F1C00   | 0x400 (1KiB) | Large Sprite Palette 3  |
| 0x500F2000   | 0x400 (1KiB) | Medium Sprite Palette 0 |
| ...          | ...          | ...                     |
| 0x500F2C00   | 0x400 (1KiB) | Medium Sprite Palette 3 |
| 0x500F3000   | 0x400 (1KiB) | Small Sprite Palette 0  |
| ...          | ...          | ...                     |
| 0x500F3C00   | 0x400 (1KiB) | Small Sprite Palette 3  |

#### Background Palette 0 Memory Map
Below is the layout for Background Palette 0, all other palettes are organized in the same fashion. 
| Base Address | Size         | Description          |
| ------------ | ------------ | -------------------- |
| 0x500FC000   | 0x4(4B)      | Palette Entry 0      |
| 0x500FC004   | 0x4(4B)      | Palette Entry 1      |
| ...          | ...          | ...                  |
| 0x500FC3F8   | 0x4(4B)      | Palette Entry 254    |
| 0x500FC3FC   | 0x4(4B)      | Palette Entry 255    |


#### Palette Entry
Below is the structure of the palette entries where A is alpha, R is red, G is green and B is blue. All values of R, G, B should be less than or equal to that of A. 
| Bit         | 31..24 | 23..16 | 15..8 | 7..0 |
| ----------- | ------ | ------ | ----- | ---- |
| Description | A      | R      | G     | B    |

### Mode Control 
The mode control register controls the mode of the video and the refresh rate of the screen. The mode bit selects between Text Mode 0 and Graphics Mode 1. The refresh rate is a down sample from the input tick clock. When set to 0 (the default after reset) the refresh will occur every 10 ticks. It is possible to down sample the refresh rate to 1 for 1 of the clock ticks, and down to 1 for every 127 clock ticks. 

#### Mode Control Register 
Below is the layout of the Mode Control Register.
| Bit         | 31..8    | 7..1    | 0    |
| ----------- | -------- | ------- | ---- |
| Description | Reserved | Refresh | Mode |

