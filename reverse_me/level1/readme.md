## level 1

# finding password

gdb 
breack main
run
info functions //on voit aue strcp est appele

gdb
break strcmp
run
on entre un mot de passe ici "gougougak"
(gdb) x/wx $esp+4               //on affiche l'addresse du premier argument de strcmp
0xffffcd70:	0xffffcd8c          
(gdb) x/s 0xffffcd8c            //on affiche la chaine de caractere contenu dasn le premier argument on voit
                                  que la chaine qu'on a donne est directement comparee ici
0xffffcd8c:	"gougougak"


(gdb) x/wx $esp+8               //on affiche l'addresse du deuxieme argument de strcmp
0xffffcd74:	0xffffcd7e
(gdb) x/s 0xffffcd7e            //on affiche le contenu de l'addresse probablement notre mot de passe
0xffffcd7e:	"__stack_check"  


