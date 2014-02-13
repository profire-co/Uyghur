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
		{:import 'dsn'=>"KsHeader.tpl" :}
	</td></tr>
	<tr><td>
		<table cellspacing=5  cellpadding=5 border=0 id="index">
			<tr class='bg_menu'>
				<th colspan='9'>
					Column Families:
				</th>
			</tr>
			{:if empty($cfDefs) :}
			<tr class='bg_list1'>
				<th width='200'>No Column Families</th>
			</tr>
			{:else:}
			<tr class='bg_menu'>
				<th>Name</th>
				<th colspan='6'>Action</th>
				<th>Column Type</th>
				<th>Default Validation Class</th>
			</tr>
			{:foreach $cfDefs as $list=>$value :}
			<tr class='bg_list{:$_%2+1:}'>
				<td><b>{:$value['name']:}</b></td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfBrowse&func=View'>
						<img src='img/table.gif' alt="Browse" title="Brose:{:$value['name']:}">
					</a>
				</td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfStructure'>
						<img src='img/structure.gif' alt="Structure" title="Structure:{:$value['name']:}">
					</a>
				</td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfSearch'>
						<img src='img/search.gif' alt="Search" title="Search:{:$value['name']:}">
					</a>
				</td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfInsert'>
						<img src='img/insert.gif' alt="Insert" title="Insert:{:$value['name']:}">
					</a>
				</td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfStructure&func=Truncate&back={:urlencode(REQUEST_URI):}'
						onClick="return confirmMsg('Do you really want to :Truncate ColumnFamily {:$value['name']:} ?')";
						>
						<img src='img/empty.gif' alt="Empty" title="Empty:{:$value['name']:}">
					</a>
				</td>
				<td width='16'>
					<a href='?ksName={:KEY_SPACE:}&cfName={:$value['name']:}
						&view=CfStructure&func=Drop&back={:urlencode(REQUEST_URI):}'
						onClick="return confirmMsg('Do you really want to :Drop ColumnFamily {:$value['name']:} ?')";
						>
						<img src='img/delete.gif' alt="Drop" title="Drop:{:$value['name']:}">
					</a>
				</td>
				<td>{:$value['column_type']:}</td>
				<td>{:$value['default_validation_class']:}</td>

			</tr>
			{:/foreach:}
			{:/if:}
		</table>
	</td></tr>
	<tr><td><hr></td></tr>
	<tr><td>
		<form action='' method="GET"  onSubmit="return confirmMsg('Do you really want to :Create Column Family ?');">
			<input type="hidden" name="view"   value="CfStructure"/>
			<input type="hidden" name="func"   value="Create"/>
			<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
			<input type="hidden" name="back"   value="{:urlencode(REQUEST_URI):}"/>
			<table cellspacing=5  cellpadding=5 border=0 id="structure">
				<tr class='bg_menu'>
					<th colspan='2'>Create new Column Family</th>
				</tr>
				<tr class='bg_list1'>
					<th>Name</th>
					<td><input type='text' name="cfName" size='30'></td>
				</tr>
				{:foreach $options as $option=>$label @$index:}
				<tr class='bg_list{:2 - $index % 2:}'>
					<th>{:$label:}</th>
					<td>
						{:if array_search($option,$useDataType) === FALSE :}
						<input type='text' name="options[{:$option:}]" size='30'>
						{:else:}
						{:html_options name=options[$option] options=$dataType :}
						{:/if:}
					</td>
				</tr>
				{:/foreach:}
				<tr><td colspan='2' align='right'>
					<input type="image" src="img/create.gif" alt="Create" title="Create">
				</td></tr>
			</table>
		</form>
	</td></tr>
	<tr><td><hr></td></tr>
	<tr><td>
		<table cellspacing=5  cellpadding=5 border=0 id="structure">
			<tr class='bg_menu'>
				<th colspan='2'>
					Setting:
				</th>
			</tr>
			{:foreach $ksDef as $option=>$item :}
			<tr class='bg_list{:$_ % 2 + 1:}'>
				{:if $option=='cf_defs':}{:continue:}{:/if:}
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
