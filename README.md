# CXSA

[!cxsa](./plasma.gif)

This project implements [Xor's Shader Arsenal](https://www.xordev.com/arsenal), but in pure C. Largely inspired by [Tsoding's C++ implementation](https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf) (see also his [video](https://www.youtube.com/watch?v=xNX9H_ZkfNE)). 

## Dependencies
You need to install [cglm](https://github.com/recp/cglm), used here for the vector computations. You also need [ffmpeg](https://www.ffmpeg.org/) in order to turn the generated image to videos.

## Installation
Just run `make checker` to generate the checker video, and `make plasma` for the plasma video. Then you can clean all the mess with `make clean`.

## Performance
This C version runs a tiny bit (4.5%) faster than the C++ version with the same compilation flags on my PC.
