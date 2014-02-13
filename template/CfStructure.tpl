<!--
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 *
-->
<table cellspacing=0 border=0 width='100%' id="table">
	<tr><td>
		{:import 'dsn'=>"CfHeader.tpl":}
	</td></tr>
	<tr><td>
		<table cellspacing=5  cellpadding=5 border=0 id="index">
			<tr class='bg_menu'>
				<th colspan='6'>
					indexes:
				</th>
			</tr>
			{:if empty($columnMetadata) :}
			<tr class='bg_list1'>
				<th width='200'>No Index</th>
			</tr>
			{:else:}
			<tr class='bg_menu'>
				<th>Column</th>
				<th>Action</th>
				<th>Validation Class</th>
				<th>Type</th>
				<th>Name</th>
				<th>Options</th>
			</tr>
			{:foreach $columnMetadata as $value:}
			<tr class='bg_list{:$_ % 2 + 1 :}'>
				<td><b>{:$value['name']:}</b></td>
				<td width='16' align='center'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&column={:$value['name']:}
						&view=CfIndex&func=Drop&back={:urlencode(REQUEST_URI):}'
						onClick="return confirmMsg('Do you really want to :Drop Index {:$value['name']:} ?')";
						>
						<img src='img/delete.gif' alt="Drop" title="Drop:{:$value['name']:}">
					</a>
				</td>
				<td>{:$value['validation_class']:}</td>
				<td>{:$value['index_type']:}</td>
				<td>{:$value['index_name']:}</td>
				<td>{:$value['index_options']:}</td>
			</tr>
			{:/foreach:}
			{:/if:}
		</table>
	</td></tr>
	<tr><td><hr></td></tr>
	<tr><td>
	<form action='' method="GET"  onSubmit="return confirmMsg('Do you really want to :Create Index ?');">
		<input type="hidden" name="view"   value="CfIndex"/>
		<input type="hidden" name="func"   value="Create"/>
		<input type="hidden" name="back"   value="{:urlencode(REQUEST_URI):}"/>
		<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
		<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
		<table cellspacing=5  cellpadding=5 border=0 id="structure">
			<tr class='bg_menu'>
				<th colspan='4'>
					Create new Index
				</th>
			</tr>
			<tr class='bg_menu'>
				<th>Column</th>
				<th>Validation Class</th>
				<th>Index Name</th>
				<td></td>
			</tr>
			<tr class='bg_list1' align='center'>
				<td><input type="text" name='column' size='30'></td>
				<td>
					<select name="dateType">
						{:foreach $dataType as $k=> $v :}
						<option value={:$k:}>{:$v:}</option>
						{:/foreach:}
					</select>
				<td><input type="text" name='name' size='30'></td>
				<td width='20'><input type="image" src="img/create.gif" alt="Create" title="Create"></td>
			</tr>
		</table>
	</form>
</td>
	</tr>
	<tr><td><hr></td></tr>
	<tr><td>
		<table cellspacing=5  cellpadding=5 border=0 id="structure">
			<tr class='bg_menu'>
				<th colspan='2'>
					Setting:
				</th>
			</tr>
			{:foreach $cfdef as $option=>$item:}
			<tr class='bg_list{:$_ % 2 + 1:}'>
				{:if $option=='column_metadata':}{:continue:}{:/if:}
				<th>
					{:ucwords(str_replace('_',' ',$option)):}
				</th>
				<td>
					{:if is_array($item) :}
					{:foreach $item as $key=>$value :}
					{:$key:}&nbsp;:&nbsp;{:$value:}<br>
					{:/foreach:}
					{:else:}
					{:$item:}
					{:/if:}
				</td>
			</tr>
			{:/foreach:}
		</table>
	</td></tr>
	<tr><td><hr></td></tr>
	

	
</table>
