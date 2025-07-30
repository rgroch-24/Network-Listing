# Network-Listing

## What is this?

I created this program without any particular reason, I was bored.
It's simple program that saves a few network addresses to a binary file, and I don't think that it's well made, but I wanted to experiment with *__some__* things.

## An idea on how it works.

  1. Program get's information from terminal and then saves it to binary file.

     1.1. Program takes mask prefix (example: 24) but saves and later displays the full mask (255.255.255.0 in this case)

     1.2. Networks given apply to Network Classes (mainly A, B and C)
  
  2. During deleting of network from file, every bit is set to 0, and then later it can be replaced by another network.
