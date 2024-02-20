
//test_fixture();


translate([25+.5,25+.5,3.5])
  clamp_connector();
translate([90-.6,26,4.6])
  rotate([0,0,270])
    fixture_support();
 translate([0,-10,0])
 cube([120,70,5]);



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
      translate([29,35,2]) cylinder(d=8,h=2);
    }
}

module fixture_support()
{
  // center
  cylinder(d=7.8,h=2,$fn=60); //TODO move to 4-5d
  
  translate([-2.5-11, -9,0])
    cube([5,18,2]);
  translate([-2.5+11, -9,0])
    cube([5,18,2]);
  
  translate([18.6,2.5,0]) nob();
  translate([-18.6,2.5,0]) nob();
  
  translate([18.6,2.5-14.5,0]) nob();
  translate([-18.6,2.5-14.5,0]) nob();
  
  translate([18.6,2.5-14.5-13.5,0]) nob();
  translate([-18.6,2.5-14.5-13.5,0]) nob();  
}

module clamp_connector()
{
  height = 23;
  base = 50;
  top = 40;
  depth = 5;
  difference()
  {
    translate([0,0,0])
      rotate([0,0,45])
        cylinder(d1=base*1.4,d2=top*1.4,h=height,$fn=4);
    translate([0,0,18])
      linear_extrude(depth+.01, convexity = 2) import("ClampFootprint.svg", center=true);
  }
}

module nob() 
{
  union() 
  {
    cylinder(d=2.8,h=6,$fn=60);
    cylinder(d=6,h=2,$fn=60);
  }
}

