for(x=[0:50:200]){
    for(y=[0:50:200]){

translate([x,y,0])
difference(){
    cylinder(100,10,10,$fn=6);
    cylinder(100,6,6,$fn=3);
    
    };
}
}
