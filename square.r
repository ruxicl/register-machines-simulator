# we will iterate with r1 through {1, .., r0} and will add r0 (r3) each time
loop0:       decjz r0 mainLoop
             inc r1
             inc r3
             decjz r4 loop0
mainLoop:    decjz r1 resetOthers
copyStep:    decjz r3 sndLoop
             inc r2
	     decjz r4 copyStep
sndLoop:     decjz r2 mainLoop
             inc r0
	     inc r3
             decjz r4 sndLoop
# reset the other registers
resetOthers: decjz r3 HALT
             decjz r4 resetOthers
