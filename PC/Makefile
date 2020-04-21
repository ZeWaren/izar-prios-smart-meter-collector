all: cracker

cracker: prios_key_cracker.c ../ST-STEVAL-FKI868V1/Src/PRIOS.c
	gcc -c -Wall -Werror -std=c99 -pedantic prios_key_cracker.c -I ../ST-STEVAL-FKI868V1/Inc
	gcc -c -Wall -Werror -std=c99 -pedantic ../ST-STEVAL-FKI868V1/Src/PRIOS.c -I ../ST-STEVAL-FKI868V1/Inc
	gcc -o prios_key_cracker prios_key_cracker.o PRIOS.o
