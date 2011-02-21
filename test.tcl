load sbattle.so
package require json

set d [json::json2dict [read [open "test.json"]]]

set f1 [dict get $d force1]
set f2 [dict get $d force2]

foreach {k v} $f1 {
	puts "f1: '$k' = '$v'"
}

foreach {k v} $f2 {
	puts "f2: '$k' = '$v'"
}

