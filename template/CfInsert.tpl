<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<form action='' method="GET" onSubmit="return confirmMsg('Do you really want to :Update Row ?');" name="up">
<table cellspacing=0 border=0 width='100%' id="table">
	<tr><td>
		{:import 'dsn'=>"CfHeader.tpl":}
	</td></tr>
	{:foreach $rowKeys as $num=>$rowKey :}
	<tr><td width='400'>
		<table cellspacing=5  cellpadding=5 border=0 id="updateList_{:$num:}">
			<tr>
				<th class='bg_menu'>
					RowKey
				</th>
				<td colspan='2'>
					<input type="text" name="rowKeys[]"  size='40'
						{:if $rowKey!='' :}readonly="readonly" value="{:$rowKey:}" {:/if:}
						>
				</td>
			</tr>
			<tr class='bg_menu'>
				<th width='160'>Column Name</th>
				<th width='160'>Column Value</th>
				<th>NULL</th>
			</tr>
			{:if !isset($list[$rowKey]) :}
			{:if empty($columnList) :}
			<tr class='bg_list1'>
				<td><input type="text" name="columns[{:$num:}][]"size="30"></td>
				<td><input type="text" name="values[{:$num:}][]" size="30"></td>
				<td>&nbsp;</td>
			</tr>
			{:else:}
			{:foreach $columnList as $column @$list:}
			<tr class='bg_list{:$list % 2 + 1:}'>
				<td><input type="text" name="columns[{:$num:}][]" size="30" value="{:$column:}"></td>
				<td><input type="text" name="values[{:$num:}][]"  size="30" ></td>
				<td><input type="checkbox" name="nulls[{:$num:}][{:$column:}]"></td>
			</tr>
			{:/foreach:}
			{:/if:}
			{:else :}
			{:foreach $list[$rowKey] as $column=>$value @$list:}
			<tr class='bg_list{:$list % 2 + 1:}'>
				<td><input type="text" name="columns[{:$num:}][]" size="30" value="{:$column:}"></td>
				<td><input type="text" name="values[{:$num:}][]" size="30" value="{:$value:}"
					{:if strpos($value,'1.0E+') ===0 :}readonly="readonly"{:/if:}
					></td>
				<td><input type="checkbox" name="nulls[{:$num:}][{:$column:}]" {:if is_null($value):}checked{:/if:}></td>
			</tr>
			{:/foreach:}
			{:/if:}
			<tr><td>
				<input type="button" value="AddColumn" onClick="CfInsertAddColumn('updateList_{:$num:}','{:$num:}');" /> 
			</td></tr>
		</table>
	</td></tr>
	{:/foreach:}
	<tr><td>
		<input type="button" value="AddRow" onClick="CfInsertAddRow('table',Array({:$columnStr:}));" /> 
	</td></tr>
	<tr><td>
		<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
		<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
		<input type="hidden" name="view"   value="{:VIEW:}"/>
		<input type="hidden" name="func"   value="Update"/>
		<input type="hidden" name="back"   value="{:$back:}"/>
		<input type="submit" value="Update" name="exec" style="font-size:12pt";>
	</form>
	</td></tr>
	

</table>
</form>