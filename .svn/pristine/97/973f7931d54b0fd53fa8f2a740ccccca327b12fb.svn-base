$fn = 40;
d = 5;

q = 0.001;

module plus()
{
  cylinder(1, d+10, d+10); 
  translate([0,0,1]) cylinder(15, d,d);
}


difference()
{
  plus();
  
  translate([d+5, 0, -q]) cylinder(1+2*q, 1.5, 1.5);
  translate([0, d+5, -q]) cylinder(1+2*q, 1.5, 1.5);  
  translate([-d-5, 0, -q]) cylinder(1+2*q, 1.5, 1.5);
  translate([0, -d-5, -q]) cylinder(1+2*q, 1.5, 1.5);  
    
  cylinder(16, d-0.5, d-0.5);
}