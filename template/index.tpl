<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<html>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <link rel="stylesheet" href="./css/common.css">
    <link rel="stylesheet" href="./css/skin.{:$MENU_skins[$MENU_skin]:}.css">
	<head>
		<title>Uyghur</title>
		<script type="text/javascript" src="./common.js"></script>
		<link rel="shortcut icon" href="img/favicon.ico">
	</head>
	<body style="margin:0px;">
		{:strip:}
		<table height='100%' width='100%' cellspacing=0 border=0 cellpadding=0>
			<tr valign='top'>
				<td class='bg_select' width='160' >
					{: import 'dsn'=>'Menu.tpl' :}
				</td>
				<td class='bg_body'>
					{: if is_null($ERROR) :}
					{: import 'dsn'=>$MAIN_TEMPLATE.'.tpl' :}
					{: else :}
					<font color='red' size='+2'>{$ERROR}</font>
					{: /if :}
				</td>
			</tr>
		</table>
			{:* import 'dsn'=>'_frame.html' :}
		{:/strip:}
	</body>
</html>