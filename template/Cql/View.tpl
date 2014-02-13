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
		{:import 'dsn'=>"../CfHeader.tpl":}
	</td></tr>
	{:if $keyEmpty == false :}
	<form action='' method="GET">
	<tr><td></td></tr>
	<tr><td>
		<table cellspacing=5  cellpadding=5 border=0>
			<tr class="bg_menu">
				<th>Query string</th>
				<th>Result Num</th>
			</tr>
			<tr class="bg_list1">
				<th>{:$query:}</th>
				<th>{:$resultNum:}</th>
			</tr>
		</table>
	</td></tr>
	<tr><th align='left'>
		ShowRowsNum
	</th></tr>
	<tr><td>
		<input type="text" name="pageLimit" style="width:50px;" value="{:$pageLimit:}" onChange='form.submit();'>
	</td></tr>
	<tr><th align='left'>
		BrowseMode
	</th></tr>
	<tr>
		<td>
			{:html_radios name='mode' options=$modeCheckboxes selected=$mode onChange='form.submit();':}
			<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
			<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
			<input type="hidden" name="view"   value="{:VIEW:}"/>
			<input type="hidden" name="func"   value="{:FUNC:}"/>
		</td>
	</tr>
	<tr><th align='left'>
		DateTypeMode
	</th></tr>
	<tr>
		<td>
			{:html_radios name='dateTime' options=$dateTimeRadio selected=$dateTime onChange='form.submit();':}
		</td>
	</tr>
	</form>
	<tr>
		<form action='' method="GET" onSubmit="return confirmMsg('Do you really want to :Delete Rows ?');">
		<td>
			<table cellspacing=5  cellpadding=5 border=0 id="dataList">
				<tr class='bg_menu'>
					<td colspan='4'></td>
					<th><i>Key</i></th>
					{:foreach $columnList as $column:}
					<th>{:$column:}</th>
					{:/foreach:}
					{:if $kvsEmpty == false :}
					{:if $mode == 'kvs' :}
					<th>value</th>
					{:else:}
					<th></th>
					{:/if:}
					{:/if:}
				</tr>
				{:foreach $keyList as $key @$list:}
				<tr class='bg_list{:$list % 2 + 1:}'
					onMouseOver="ColorMouseIn(this,'dataList')"
					onMouseOut="ColorMouseOut(this,'dataList')"
					>
					<td>
						<input type="checkbox" name="rowKeys[]" value="{:$key:}" {:if $check:}checked{:/if:}>
					</td>
					<td>
						<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view=CfBrowse
							&func=Detail&rowKey={:$key:}&back={:urlencode(REQUEST_URI):}';
							>
							<img src="img/detail.gif" alt="Detail" title="Detail">
						</a>
					</td>
					<td>
						<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view=CfInsert
							&rowKeys[]={:$key:}&back={:urlencode(REQUEST_URI):}';
							>
							<img src="img/edit.gif" alt="Edit" title="Edit">
						</a>
					</td>
					<td>
						<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view={:VIEW:}&func=Delete
							&rowKeys[]={:$key:}&back={:urlencode(REQUEST_URI):}' onClick="return confirmMsg('Do you really want to :Delete Row ?');"
							>
							<img src="img/delete.gif" alt="Delete" title="Delete">
						</a>
					</td>
					<td><i>{:$key:}</i></td>
					{:foreach $columnList as $column:}
					<td nowrap {:if array_search($column,$dateColumn)!==FALSE:}class="datetime"{:/if:}>
						{:if isset($rdbmsList[$key][$column]):}
						{:$rdbmsList[$key][$column]:}
						{:else:}
						&nbsp;
						{:/if:}
					</td>
					{:/foreach:}
					{:if $kvsEmpty == false :}
					<td width='100%'>
						{:if count($kvsList[$key]) > 0 :}
						<table class='rest_table'  cellpadding=2 width='100%'>
							{:foreach $kvsList[$key] as $column=>$value @$kv:}
							<tr>
								<td class='rest_menu' width='50%'>{:$column:}</td>
								<td class='rest_{:$kv % 2 +1 :}' width='50%' nowrap>{:$value:}</td>
							</tr>
							{:/foreach:}
						</table>
						{:else:}
						&nbsp;
						{:/if:}
					</td>
					{:/if:}
				</tr>
				{:/foreach:}
			</table>
		</td>
	</tr>
	<tr>
		<td>
			<table><tr>
				<td>
					&nbsp;&nbsp;
					<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view={:VIEW:}&func={:FUNC:}
						&pageLimit={:$pageLimit:}&mode={:$mode:}&dateTime={:$dateTime:}&page={:$page:}&check=1'
						>
						Check All
					</a>
					&nbsp;/&nbsp;
					<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view={:VIEW:}&func={:FUNC:}
						&pageLimit={:$pageLimit:}&mode={:$mode:}&dateTime={:$dateTime:}&page={:$page:}'
						>
						Uncheck All
					</a>
				</td>
				<td>
					<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
					<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
					<input type="hidden" name="view"   value="{:VIEW:}"/>
					<input type="hidden" name="func"   value="Delete"/>
					<input type="hidden" name="back"   value="{:REQUEST_URI:}"/>
					&nbsp;<input type="image" src="img/delete.gif" alt="Delete" title="Delete" name="Delete">
					&nbsp;<input type="image" src="img/edit.gif" alt="Edit" title="Edit" name="Edit">
				</td>
			</tr></table>
			
		</td>
	</tr>
	</form>
	<tr height='30'>
		<td align='center'>
			<b>{:if $page > 1:}
			&nbsp;&nbsp;<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view={:VIEW:}&func={:FUNC:}
				&pageLimit={:$pageLimit:}&mode={:$mode:}&dateTime={:$dateTime:}&page={:$page-1:}'
				><&nbsp;Prev
			</a>
			{:/if:}
			&nbsp;&nbsp;{:$page:}Page({:$beginCount:}-{:$endCount:})
			{:if $resultNum > ($page * $pageLimit) :}
			&nbsp;&nbsp;<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view={:VIEW:}&func={:FUNC:}
				&pageLimit={:$pageLimit:}&mode={:$mode:}&dateTime={:$dateTime:}&page={:$page+1:}'
				>Next&nbsp;>
			</a>
			{:/if:}
			</b>
		</td>
	</tr>
	{:else:}
	<tr><td>data not found</td></tr>
	{:/if:}
</table>
