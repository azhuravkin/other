<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Welcome to Web Server</title>
<script type="text/javascript" src="/jwplayer/jwplayer.js"></script>
</head>
<body bgcolor="white" text="black">
<table>
<?php
$id = 1;

function dirent($dir) {
    $ret = array();

    if ($d = opendir($dir)) {
	while (($e = readdir($d)) !== false) {
	    if (is_dir("$dir/$e") && $e != '.' && $e != '..' && $e != 'images') {
		$ret[] = $e;
	    } elseif (is_file("$dir/$e") && ($dot = strrchr($e, '.')) !== false) {
		$ext = substr($dot, 1);

		switch ($ext) {
		    case 'flv':
		    case 'mp4':
		    case 'm4v':
		    case 'f4v':
		    case 'mov':
			$ret[] = $e;
			break;
		    default:
			continue 2;
		}
	    }
	}
    }

    sort($ret);

    return($ret);
}

function show_next($dir, $files) {
    global $id;

    if (($first = array_shift($files)) !== NULL) {
	$img = substr($first, 0, strrpos($first, '.')) . '.jpg';

	if (!file_exists("$dir/images/$img")) {
	    system("ffmpeg -y -i \"$dir/$first\" -ss 15 -vframes 1 -an -sameq -f image2 \"$dir/images/$img\"");
	}

	echo <<<END
<div id="clip$id">Loading the player...</div>
<script type="text/javascript">
    jwplayer("clip$id").setup({
	height: 300,
	width: 480,
	file: "$dir/$first",
	image: "$dir/images/$img",
	title: "$first"
    });
</script>
END;
	$id++;
    } else {
	echo '&nbsp;';
    }
}

if (isset($_GET['d'])) {
    $dir = urldecode($_GET['d']);
    $files = dirent("lf/$dir");

    $rows = ceil(count($files) / 2);

    for ($i = 0; $i < $rows; $i++) {
	echo '<tr><td>';
	show_next("lf/$dir", &$files);
	echo '</td><td>';
	show_next("lf/$dir", &$files);
	echo '</td></tr>';
    }
} else {
    $dirs = dirent("lf");

    foreach ($dirs as $dir) {
	$count = count(dirent("lf/$dir"));

        print "<tr><td><a href='?d=" . urlencode($dir) . "'>$dir</a></td><td align='right'>$count</td></tr>\n";
    }
}
?>
</table>
</body>
</html>
