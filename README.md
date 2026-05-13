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
