all : encoder decoder

encoder :
		gcc -o base64encoder base64encoder.c -g2 -O2

decoder :
		gcc -o base64decoder base64decoder.c -g2 -O2