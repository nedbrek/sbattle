load sbattle.so
package require json

for {set i 0} {$i < 5000} {incr i} {
	battle fight
}

set res [battle results]

set numTrials [lindex $res 0]
set bugsWin   [lindex $res 1]
set impsWin   [lindex $res 2]
set sumRounds [lindex $res 8]

puts [format "Imps: %d\t Bugs: %d, Draw: %d" \
	$impsWin $bugsWin [expr $numTrials - $impsWin - $bugsWin]]

puts "Imps lost due to hull failure: [lindex $res 4]"
puts "Imps lost due to weapons failure: [lindex $res 6]"
puts "Bugs lost due to hull failure: [lindex $res 3]"
puts "Bugs lost due to weapons failure: [lindex $res 5]"
puts "Bugs lost due to weapons unable to breach enemy shields: [lindex $res 7]"
puts [format "Total number of rounds fought:%d Average: %f" \
	$sumRounds [expr $sumRounds / (1.0 * $numTrials)]]

exit
set d [json::json2dict [read [open "test.json"]]]

set f1 [dict get $d force1]
set f2 [dict get $d force2]

foreach {k v} $f1 {
	puts "f1: '$k' = '$v'"
}

foreach {k v} $f2 {
	puts "f2: '$k' = '$v'"
}

