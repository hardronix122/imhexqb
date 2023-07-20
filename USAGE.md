# ImHexQb Usage

# Installation:
Go to Help -> About, check plugins paths and extract downloaded archive into one of those directories!

# Usage
It's quite intuitive, as any selected chunk of bytes will be instantly decompiled, press Compile to compile it

If you need to see checksum names:
1. Find where qb table begins (usually it's a dense chunk of ASCII text, which begins from 0x01 0x2B, followed by 4 bytes and text, then repeats)
2. Select 0x01 or 0x2B byte
3. Scroll to the bottom
4. Enable greedy symbol capture
5. Shift right-click on the last or pre-last byte of the script
6. Turn off greedy symbol capture

Now, you should be able to see all the names ;3