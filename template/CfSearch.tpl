<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<form action='' method="GET">
<table cellspacing=0 border=0 width='100%' id="table">
	<tr><td>
		{:import 'dsn'=>"CfHeader.tpl":}
	</td></tr>
	<tr><td width='400'>
		<table cellspacing=5  cellpadding=5 border=0 id="input">
			<tr>
				<th class='bg_menu' width='120'>
					RowKey
				</th>
				<td class='bg_list1'>
					<input type="text" name="rowKeys[]"  size='40'>
				</td>
			</tr>
		</table>
	</td></tr>
	<tr><td>
		<input type="button" value="AddRowKey" onClick="CfSearchAddRowky('input');" /> 
	</td></tr>
	<tr><td>
		<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
		<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
		<input type="hidden" name="view"   value="CfBrowse"/>
		<input type="hidden" name="func"   value="View"/>
		<input type="submit" value="Search" name="exec" style="font-size:12pt";>
	</form>
	</td></tr>
</table>
</form>