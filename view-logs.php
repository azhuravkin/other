<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<style type="text/css">
body {font-family:Tahoma,Verdana,Arial;color:#000000;background-color:#ffffff;}
.data {font-family:Tahoma,Verdana,Arial;color:#000000;font-size:12px;background-color:lavender;border-right:1px solid #6A5ACD;border-bottom:1px solid #6A5ACD;}
</style>
<table align='center'>

<?php
$path = array("/var/log/nginx/", "/var/log/php-fpm/");
$step = 20;
$offset = 0;

if ($_GET['path'] != "" && $_GET['file'] != "") {
    $offset = $_POST['offset'];

    if ($_POST['goto'] == ">>") {
	$offset++;
    } elseif ($_POST['goto'] == "<<") {
	$offset--;
	if ($offset < 0) {
	    $offset = 0;
	}
    } elseif ($_POST['goto'] == "reset") {
	$offset = 0;
    }

    if ($fh = fopen($path[$_GET['path']] . $_GET['file'], "r")) {
	for ($line = 0; !feof($fh); $line++) {
	    $map[$line] = ftell($fh);
	    fgets($fh);
	}

	fseek($fh, $map[$line - $step - 1 - $offset * $step]);

	for ($num = 0; !feof($fh) && ($num < $step); $num++) {
	    $lines[$num] = fgets($fh);
	}

	fclose($fh);

	for ($i = $num - 1; $i >= 0; $i--) {
	    if (($realnum = $line - ($num - $i) - $offset * $step) < 1) {
		break;
	    }
	    echo "<tr><td class='data'>" . $realnum . "</td>";
	    echo "<td class='data'>" . $lines[$i] . "</td></tr>";
	}

	echo "<form method='POST'><tr><td align='center' colspan='2'>";
	echo "<input type='submit' name='goto' value='<<'>";
	echo "<input type='submit' name='goto' value='reset'>";
	echo "<input type='submit' name='goto' value='>>'>";
	echo "<input type='hidden' name='offset' value=" . $offset . "></td></tr></form>";
    } else {
	echo "Error opening file: " . $path . $_GET['file'];
    }
} else {
    echo "<tr>\n";

    for ($i = 0; isset($path[$i]); $i++) {
	if ($dh = opendir($path[$i])) {
	    echo "<td valign='top'><table><tr><th class='data'>" . $path[$i] . "</th></tr>\n";

	    $files = scandir($path[$i]);

	    for ($n = 0; isset($files[$n]); $n++) {
		if (is_file($path[$i] . $files[$n])) {
		    echo "<tr><td class='data'><a href='?path=" . $i . "&file=" . $files[$n] . "'>" . $files[$n] . "</a></td></tr>\n";
		}
	    }
	    echo "</table></td>\n";
	    closedir($dh);
	}
    }
    echo "</tr>\n";
}
?>
</table>
