
    """
    LogTemp:Warning: y:0.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-180.000000
LogTemp:Warning: y:35.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-155.000000
LogTemp:Warning: y:70.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-130.000000
LogTemp:Warning: y:105.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-105.000000
LogTemp:Warning: y:140.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-80.000000
LogTemp:Warning: y:175.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-55.000000
LogTemp:Warning: y:210.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-30.000000
LogTemp:Warning: y:245.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:-5.000000
LogTemp:Warning: y:280.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:20.000000
LogTemp:Warning: y:315.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:45.000000
LogTemp:Warning: y:350.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:70.000000
LogTemp:Warning: y:385.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:95.000000
LogTemp:Warning: y:420.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:120.000000
LogTemp:Warning: y:455.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:145.000000
LogTemp:Warning: y:490.000000 h:35.000000 mrangy:180.000000 scale:25.000000 range:360.000000 calc:170.000000
"""



#float Yrange = FMath::Abs(pFg->maxRangeY - pFg->minRangeY)/pFg->scaleY;
#pFg->cellHeightY = (int)(pFg->height / (float)Yrange)  ;// = 




yval=[0,35.000000,  70.000000, 105.000000,140.000000,175.000000,210.000000,245.000000,280.000000,315.000000,350.000000,385.000000,420.000000,455.000000,490.000000]
scaleY=25
maxRangeY=15
minRangeY=-15
height=500
Yrange=abs(maxRangeY-minRangeY)/float(scaleY)
range=abs(maxRangeY-minRangeY)

cellHeightY=height/Yrange
#cellHeightY=height/(abs(maxRangeY-minRangeY)/scaleY)
[  t(height-x) for x in yval ]


def t(lY):
    return (lY / cellHeightY)*scaleY + minRangeY

#float ylevel = (line.Y / fg.cellHeightY)*fg.scaleY + fg.minRangeY;

#[  t(x) for x in yval ]


axis=[ t(x) for x in yval ]