awk '
    NR>1{
        arr[$1]   += $2
        count[$1] += 1
    }
    END{
 	  for (a in arr) {
	  print a " " arr[a] / count[a]

        }
    }
' temp_output/combine_dir.txt
