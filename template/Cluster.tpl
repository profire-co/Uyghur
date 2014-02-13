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
		<table cellspacing=0 border=0>
			<tr height='40px'>
				<td width='5'></td>
				<td width='18'><img src='img/cluster.gif' align="absmiddle"></td>
				<th align='left'>
					<a href='?clusterName={:CLUSTER_NAME:}&view=Cluster'>ClusterName:{:CLUSTER_NAME:}</a>
				</th>
			</tr>
		</table>
		<tr><td>
		<table cellspacing=5  cellpadding=5 border=0 id="index">
			<tr class='bg_menu'>
				<th colspan='10'>
					Key Spaces:
				</th>
			</tr>
			{:if empty($ksDefs) :}
			<tr class='bg_list1'>
				<th width='200'>No Key Spaces</th>
			</tr>
			{:else:}
			<tr class='bg_menu'>
				<th>Name</th>
				<th colspan='2'>Action</th>
				<th>Strategy Class</th>
				<th>Strategy Options</th>
				<th>Durable Writes</th>
			</tr>
			{:foreach $ksDefs as $ksDef:}
			<tr class='bg_list{:$_ % 2 + 1:}'>
				<td><b>{:$ksDef['name']:}</b></td>
				<td width='16'>
					<a href='?ksName={:$ksDef['name']:}&view=KsStructure'>
						<img src='img/keyspace.gif' alt="Structure" title="Structure:{:$ksDef['name']:}">
					</a>
				</td>
				<td width>
					<a href='?ksName={:$ksDef['name']:}&view=KsStructure&func=Drop&back={:urlencode(REQUEST_URI):}'
						onClick="return confirmMsg('Do you really want to :Drop KeySpace ?')";
						>
						<img src='img/delete.gif' alt="Drop" title="Drop:{:$ksDef['name']:}">
					</a>
				</td>
				<td>{:$ksDef['strategy_class']:}</td>
				<td>
					{:foreach $ksDef['strategy_options'] as $key => $value  :}
					{:$key:}:{:$value:}<br>
					{:/foreach:}
				</td>
				<td>{:$ksDef['durable_writes']:}</td>
			</tr>
			{:/foreach:}
			{:/if:}
		</table>
	</td></tr>
	<tr><td><hr></td></tr>
	<tr><td>
		<form action='' method="GET"  onSubmit="return confirmMsg('Do you really want to :Create KeySpace ?');">
			<input type="hidden" name="view"   value="KsStructure"/>
			<input type="hidden" name="func"   value="Create"/>
			<input type="hidden" name="back"   value="{:urlencode(REQUEST_URI):}"/>
			<table cellspacing=5  cellpadding=5 border=0 id="structure">
				<tr class='bg_menu'>
					<th colspan='4'>
						Create new KeySpace
					</th>
				</tr>
				<tr class='bg_menu'>
					<th>KeySpace Name</th>
					<th>Strategy Class</th>
					<th>Replication Factor</th>
					<td></td>
				</tr>
				<tr class='bg_list1' align='center'>
					<td><input type="text" name='name' size='30'></td>
					<td>{:html_options name=strategy_class options=$strategy_class :}</td>
					<td><input type="text" name='replication_factor' size='5'></td>
					<td width='20'><input type="image" src="img/create.gif" alt="Create" title="Create"></td>
				</tr>
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
			<tr class='bg_list1'>
				<th>Name</th>
				<td>{:CLUSTER_NAME:}</td>
			</tr>
			<tr class='bg_list2'>
				<th>Partitioner</th>
				<td>{:$partitioner:}</td>
			</tr>
			<tr class='bg_list1'>
				<th>Snitch</th>
				<td>{:$snitch:}</td>
			</tr>
			<tr class='bg_list2'>
				<th>Ip</th>
				<td>{:$ip:}</td>
			</tr>
			<tr class='bg_list1'>
				<th>Port</th>
				<td>{:$port:}</td>
			</tr>
		</table>
	</td></tr>
	<tr><td><hr></td></tr>
	

	
</table>
