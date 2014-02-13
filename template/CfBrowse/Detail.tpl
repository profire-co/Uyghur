<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<table cellspacing=0 border=0>
	<tr><td>
		{: import 'dsn'=>"CfHeader.tpl" :}
	</td></tr>
	<tr><td></td></tr>
	<tr><th align='left'>
		Detail Info
	</th></tr>
	<tr>
		<td>
			<table cellspacing=5  cellpadding=5 border=0 id="dataList">
				<tr class='bg_menu'>
					<th>Name</th>
					<th>Value</th>
					<th>Timestamp</th>
					<th>Timestamp(date)</th>
					<th>Ttl</th>
				</tr>
				{:foreach $result as $column:}
				<tr
					class='bg_list{:$_ % 2 + 1:}'
					onMouseOver="ColorMouseIn(this,'dataList')"
					onMouseOut="ColorMouseOut(this,'dataList')"
					>
					<td>{:$column['name']:}</td>
					<td>{:$column['value']:}</td>
					<td>{:$column['timestamp']:}</td>
					<td>{:$column['timestamp_date']:}</td>
					<td>{:$column['ttl']:}</td>
				</tr>
				{:/foreach:}
				<tr><td><form>
					<input type="button" value="Back" onClick="location.href='{:$back:}'">
				</form></td></tr>
			</table>
		</td>
	</tr>
</table>
