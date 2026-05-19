# terraria-world-file-parser
Read a `.wld` save file and output world stats (size, biomes, ore counts, bosses defeated)

Have fun!

## Obtaining a .wld save file

### macOS

```
cd Library/Application\ Support/Terraria
cd Worlds
```

Get the relevant `.wld` file

We know that the file is a binary file, because `file -b --mime Fleek_Canyon_of_Mercy.wld` returns `application/octet-stream; charset=binary` 
.
## Parsing the file

### Byte Layout

| Offset | Size | Type      | Description                      | Value          |
|--------|------|-----------|----------------------------------|----------------|
| 0x00   | 4    | uint32_t  | File version                     | 279            |
| 0x04   | 7    | char[]    | Magic number                     | "relogic"      |
| 0x0B   | 1    | char      | File type                        | 2 = world file |
| 0x0C   | 4    | uint32_t  | Revision number                  |                |
| 0x10   | 8    | uint64_t  | Is favourite                     |                |
| 0x18   | 2    | short     | Number of pointers               | N              |
| 0x1A   | 4×N  | int32_t[] | Section offset table (N entries) |                |


The first 4 bytes (`int`) is allocated to the file version.  
The next 7 bytes are the magic number. It's equivalent ASCII value should be equal to "relogic", Terraria's developer.  
The next byte (`0x0B`) indicates the file type. A value of 2 confirms that we are dealing with a World file.  

### Running C
```
clang file_parser.c -o file_parser -Wall
./file_parser
```
