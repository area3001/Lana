
test_fixture();

module test_fixture()
{
  raised = 23;
  
  translate([4*1.4,4*1.4,15+3.5])
    difference()
    {
      translate([20,20,-15])
        rotate([0,0,45])
          cylinder(d1=50*1.4,d2=40*1.4,h=raised,$fn=4);

      translate([1.5,2.5,raised-20])
        linear_extrude(5.01, convexity = 2) import("ClampFootprint.svg");
    }

  cube([120,52,5]);
    
  translate([125,1,2.5])
    rotate([0,0,90])
    {
        translate([3.5,60-4.3-1.5,2])  {
        nob();
        translate([10.6+3,0,0])
          nob();
        translate([25+3,0,0])
          nob();
      }

      translate([3.5,60-4.3-1.5-40.2+3,2])  {
        nob();
        translate([10.6+3,0,0])
          nob();
        translate([25+3,0,0])
          nob();
      }
    }
    
  translate([90,30,0])
  cylinder(d=8,h=6);
}

module nob() 
{
  union() 
  {
    cylinder(d=2.8,h=6,$fn=60);
    cylinder(d=6,h=2,$fn=60);
  }
}

