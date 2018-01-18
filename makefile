all: virus host seed run

virus : virus.c
	gcc -o virus virus.c 

host : host.c
	gcc -o host host.c 

seed : seed.c
	gcc -o seed seed.c 


run:
	cp virus seed
	printf '\xde\xad\xbe\xef' >> seed
	cat host >> seed
	printf '\xbe\xef\xde\xad' >> seed
	cat virus.c >> seed

clean: 
	rm virus 
	rm host 
	rm seed 
	
