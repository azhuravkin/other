<?php
	$ch = curl_init();

	curl_setopt($ch, CURLOPT_URL, 'http://example.com'.$_SERVER['REQUEST_URI']);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_ENCODING , 'gzip');
	curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2 GTB5');

	$page = curl_exec($ch);
	$content_type = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
	$content_length = curl_getinfo($ch, CURLINFO_CONTENT_LENGTH_DOWNLOAD);

	curl_close($ch);

	header('Content-Type: '.$content_type);

	if ($content_length != -1) {
		header('Content-Length: '.$content_length);
	}

	echo str_replace('old.site.com', 'new.site.com', $page);
?>
