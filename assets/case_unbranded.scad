$m_length = 256;
$m_width = 64;
$m_height = 15;

$margin_x = 5;
$margin_y = 5;

$margin = ([$margin_x,$margin_y,0]);

$wall_strength = 5;
$wall = ([$wall_strength,$wall_strength,0]);

$box_l = $m_length+$margin_x+$wall_strength;
$box_w = $m_width+$margin_y+$wall_strength;
$box_h = 30;

$matrix = ([$m_length,$m_width,$box_h]);

$stand = ([5,1,5]);


font = "Liberation Sans";
letter_size = 10;
letter_height = 1;



difference() {
    // full case
    roundedcube($matrix+$wall+$margin,false,4,"z");
    //remove matrix+margin
    
    translate([$wall_strength/2,$wall_strength/2,2]) { 
        cube($matrix+$margin);
    }
    //remove matrix
    translate([$wall_strength, $wall_strength,0]) { 
        cube($matrix);
    }
    
    // insert
    translate([0,2,$box_h-3]) {
         cube([$box_l-2.5,$box_w-3,2]);
    }
    translate([$box_l-3,2.5,$box_h-3]) {
        cube([2,$box_w-4,2]);
    }
    translate([0,0,$box_h-3]) {
        cube([$wall_strength-2,$box_w,6]);
    }
    

    
}


//STAND
translate([$box_l/6,$box_w,$box_h/5]) {
    difference() {
        cube($stand);
        translate([0.5,0,0.5]) {
            cube($stand-[1,0,1]);
        }
    }
}
translate([$box_l/6,$box_w,$box_h-$box_h/5]) {
    difference() {
        cube($stand);
        translate([0.5,0,0.5]) {
            cube($stand-[1,0,1]);
        }
    }
}
translate([$box_l-$box_l/6,$box_w,$box_h-$box_h/5]) {
    difference() {
        cube($stand);
        translate([0.5,0,0.5]) {
            cube($stand-[1,0,1]);
        }
    }
}
translate([$box_l-$box_l/6,$box_w,$box_h/5]) {
    difference() {
        cube($stand);
        translate([0.5,0,0.5]) {
            cube($stand-[1,0,1]);
        }
    }
}
translate([$box_l/2,$box_w,$box_h/2]) {
    difference() {
        cube($stand);
        translate([0.5,0,0.5]) {
            cube($stand-[1,0,1]);
        }
    }
}


// BACKPLANE

/*
$backplane_dim = $matrix+$wall+$margin;
translate([0,0,7]) { 
$backplane = $backplane_dim-[0,0,3.8];

      translate([0.4,0.4,-1.5]) {
        cylinder($fn=100, r=0.15);
    }
        translate([0.4,$box_w+1-0.4,-1.5]) {
        cylinder($fn=100, r=0.15);
    }
        translate([$box_l+1-0.4,0.4,-1.5]) {
        cylinder($fn=100, r=0.15);
    }
        translate([$box_l+1-0.4,$box_w+1-0.4,-1.5]) {
        cylinder($fn=100, r=0.15);
    }  
    difference() {
        roundedcube($backplane,false,0.4,"z");
        translate([$box_l/2+1,$box_w+0.25,-1]) {
            cylinder($fn=100, r=0.15);
    }  
        translate([$box_l/2+0.85,$box_w+0.25,-0.75]) {
            cube([0.3,1.5,0.5]);
    } 
 
        translate([0,0,-0.3]) {   
            cube($backplane+[0,0,1]);
        }
    }

}
*/

//SPACER
/*translate([0.5,0.5,7]) {
  // difference() {
        cube($matrix+$margin-[0,0,2]);
    //    translate(0.5,0.5,0) {
     //       cube($matrix-[0,0,2]);
        //}
   // }
}
*/

/*difference() 
   {
       cube([10,5,10]);
       translate([0.5,0.5,0.5]) {
           cube([10,5,10]);
       }
       translate([2.5,2.5,0]) {
       cylinder($fn = 100, r=0.2);
       }
}*/






module letter(l) {
	// Use linear_extrude() to make the letters 3D objects as they
	// are only 2D shapes when only using text()
	linear_extrude(height = letter_height) {
		text(l, size = letter_size, font = font, halign = "center", valign = "center", $fn = 16);
	}
}

// Higher definition curves
$fs = 0.01;

module roundedcube(size = [1, 1, 1], center = false, radius = 0.5, apply_to = "all") {
	// If single value, convert to [x, y, z] vector
	size = (size[0] == undef) ? [size, size, size] : size;

	translate_min = radius;
	translate_xmax = size[0] - radius;
	translate_ymax = size[1] - radius;
	translate_zmax = size[2] - radius;

	diameter = radius * 2;

	module build_point(type = "sphere", rotate = [0, 0, 0]) {
		if (type == "sphere") {
			sphere(r = radius);
		} else if (type == "cylinder") {
			rotate(a = rotate)
			cylinder(h = diameter, r = radius, center = true);
		}
	}

	obj_translate = (center == false) ?
		[0, 0, 0] : [
			-(size[0] / 2),
			-(size[1] / 2),
			-(size[2] / 2)
		];

	translate(v = obj_translate) {
		hull() {
			for (translate_x = [translate_min, translate_xmax]) {
				x_at = (translate_x == translate_min) ? "min" : "max";
				for (translate_y = [translate_min, translate_ymax]) {
					y_at = (translate_y == translate_min) ? "min" : "max";
					for (translate_z = [translate_min, translate_zmax]) {
						z_at = (translate_z == translate_min) ? "min" : "max";

						translate(v = [translate_x, translate_y, translate_z])
						if (
							(apply_to == "all") ||
							(apply_to == "xmin" && x_at == "min") || (apply_to == "xmax" && x_at == "max") ||
							(apply_to == "ymin" && y_at == "min") || (apply_to == "ymax" && y_at == "max") ||
							(apply_to == "zmin" && z_at == "min") || (apply_to == "zmax" && z_at == "max")
						) {
							build_point("sphere");
						} else {
							rotate = 
								(apply_to == "xmin" || apply_to == "xmax" || apply_to == "x") ? [0, 90, 0] : (
								(apply_to == "ymin" || apply_to == "ymax" || apply_to == "y") ? [90, 90, 0] :
								[0, 0, 0]
							);
							build_point("cylinder", rotate);
						}
					}
				}
			}
		}
	}
}