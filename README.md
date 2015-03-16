# SE-314-Computer-Graphics
Computer Graphic Learning, with OpenGL mainly


### Important Basic idea - Rendering Pipeline Details
| step  |  Name  | Todo  |
|-------|--------|-------|
| 1 | model the object  | x^2+y^2=2 or y=f(x).. |
| 2 | model translation | change the location of the object in the coordinate system  |
| 3 | view point transition | change the view point |
| 4 | apply the light source  | according to every point and the meterial of the object |
| 5 | projection transform  | map the object from 3D to 2D screen |
| 6 | clipping  | some point map drop out of the screen, remove them  | 
| 7 | scan and transform  | choose the pixels to show graphic (from continous to discrete)|
| 8 | texture mapping | like adding an image to the object  |
| 9 | depth checking  | only display the point that in the upper layer  |
| 10  | blending  | some object may be transparent, so objects behind them show be reconsiderated |
| 11  | frame buffering | store the pixels information to the frame buffer, usually use the double buffering tech |
