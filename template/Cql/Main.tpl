<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<form action='' method="POST">
<table cellspacing=0 border=0 width='100%' id="table">
	<tr><td>
		{:import 'dsn'=>"../CfHeader.tpl":}
	</td></tr>
	<tr><td width='400'>
		<table cellspacing=5  cellpadding=5 border=0 id="input">
			<tr><th class='bg_menu'>
				Run CQL query
			</th></tr>
			<tr><th>
				{:if !is_null($error) :}
				<font color='red'>{:$error:}</font>
				{:else if !is_null($result) :}
				<font color='blue'>Query Succeed</font>
				{:/if:}
			</th></tr>
			<tr>
				<td class='bg_list1'>
					<textarea name="query" cols="60" rows="10">{:$query:}</textarea>
				</td>
			</tr>
		</table>
	</td></tr>
	<tr><td>
		<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
		<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
		<input type="hidden" name="view"   value="Cql"/>
		<input type="hidden" name="func"   value="Query"/>
		<input type="submit" value="Query" name="exec" style="font-size:12pt";>
	</form>
	</td></tr>
</table>
</form>