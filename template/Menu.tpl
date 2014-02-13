<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<table border='0'>
	<tr>
		<td colspan='2'><img src='img/logo.png' alt="logo"></td>
	</tr>
	<tr>
		<th colspan='2'>Server</th>
	</tr>
	<tr>
		<td colspan='2'>
			<form action='' method="GET">
				<select name="server" onChange='form.submit();'>
					{:foreach $MENU_servers as $k=>$v:}
					<option value="{:$k:}" {:code if(SERVER==$k)echo('selected'):}>{:$v:}</option>
					{:/foreach:}
				</select>
			</form>
		</td>
	</tr>

	{:if $MENU_ksOptions:}
	<tr>
		<th colspan='2'>KeySpace</th>
	</tr>
	<tr>
		<td colspan='2'>
			<form action='' method="GET">
				<input type="hidden" name="view"   value="KsStructure"/>
				<select name="ksName" onChange='form.submit();'>
					{:foreach $MENU_ksOptions as $k=>$v:}
					<option value="{:$k:}" {:code if(KEY_SPACE==$k)echo('selected'):}>{:$v:}</option>
					{:/foreach:}
				</select>
			</form>
		</td>
	</tr>
	{:/if:}
	{:if $MENU_ksDef:}
	<tr><th colspan='2' align='left'>
		<a href='?ksName={:KEY_SPACE:}&view=KsStructure'>{:$MENU_ksDef->name:}&nbsp;({:count($MENU_ksDef->cf_defs):})</a>
	</th></tr>
	<tr><td height='10'></td></tr>
	{:/if:}
	{:foreach $MENU_cfList as $key=>$value:}
	<tr class={:if $value==COLUMN_FAMILY:}'menu_cf_active'{:else:}'menu_cf'{:/if:}>
		<td width='18' >
			<a href='?ksName={:KEY_SPACE:}&cfName={:$value:}&view=CfBrowse&func=View'><img src='img/table.gif' alt="Browse" title="Browse:{:$value:}"></a>
		</td>
		<td>
			<a   href='?ksName={:KEY_SPACE:}&cfName={:$value:}&view=CfStructure'>{:$value:}</a>
	</td></tr>
	{:/foreach:}
	<tr height='100'><th></th></tr>
	<tr>
		<td colspan='2'>
			<form action='' method="GET">
				<b>skin:</b>
				<select name="skin" onChange='form.submit();'>
					{:foreach $MENU_skins as $k=>$v:}
					<option value="{:$k:}" {:code if($MENU_skin==$k)echo('selected'):}>{:$v:}</option>
					{:/foreach:}
				</select>
			</form>
		</td>
	</tr>

</table>
