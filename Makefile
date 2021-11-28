logfile: play referee player
	touch logfile
	
play: play.c 
	gcc -o play play.c 

player: player.c 
	gcc player.c -o player

referee: referee.c
	gcc referee.c -o referee

clean:	
	rm play
	rm player
	rm referee
	
