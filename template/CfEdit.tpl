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
		{:import "dsn"=>"CfHeader.tpl":}
	</td></tr>
	<tr><td>
		<form action='' method="GET"  onSubmit="return confirmMsg('Do you really want to :Edit Column Family ?');">
			<input type="hidden" name="view"   value="CfEdit"/>
			<input type="hidden" name="func"   value="Edit"/>
			<input type="hidden" name="ksName" value="{:KEY_SPACE:}"/>
			<input type="hidden" name="cfName" value="{:COLUMN_FAMILY:}"/>
			<table cellspacing=5  cellpadding=5 border=0 id="structure">
				<tr class='bg_menu'>
					<th colspan='2'>Edit Column Family : {:COLUMN_FAMILY:}</th>
				</tr>
				<tr class='bg_list1'>
					<th>Name</th>
					<td><input type='text' name="cfName" value='{:COLUMN_FAMILY:}' size='30' disabled></td>
				</tr>
				{:foreach $options as $option=>$label:}
				<tr class='bg_list{:$_ % 2:}'>
					<th>{:$label:}</th>
					<td>
						{:if array_search($option,$useDataType) === FALSE :}
						<input type='text' name="options[{:$option:}]" value='{:$cfdef[$option]:}' size='70'>
						{:else:}
						{:html_options options=$dataType selected=$cfdef[$option] name=options[$option]:}
						{:/if:}
					</td>
				</tr>
				{:/foreach:}
				<tr><td colspan='2' align='right'>
					<input type="image" src="img/edit.gif" alt="Edit" title="Edit">
				</td></tr>
			</table>
		</form>
	</td></tr>
	

	
</table>
