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
	<tr height='40px' align='left'>
		<td rowspan='2' width='5'></td>
		<td width='18'><img src='img/cluster.gif' align="absmiddle"></td>
		<th>
			<a href='?clusterName={:CLUSTER_NAME:}&view=Cluster'>ClusterName:{:CLUSTER_NAME:}</a>
		</th>
		<td width='20' align='center'><img src="img/ltr.gif"  align="absmiddle"></td>
		<td width='18'><img src='img/keyspace.gif' align="absmiddle"></td>
		<th>
			<a href='?ksName={:KEY_SPACE:}&view=KsStructure'>KeySpace:{:KEY_SPACE:}</a>
		</th>
		<td width='20' align='center'><img src="img/ltr.gif"  align="absmiddle"></td>
		<td width='18'><img src='img/structure.gif' align="absmiddle"></td>
		<th>
			<a href='?ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}&view=CfStructure'>ColumnFamily:{:COLUMN_FAMILY:}</a>
		</th>
	</th></tr>
	<tr><td colspan='10'>
		<ul id="tablist">
			<li {:if VIEW == 'CfBrowse':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'CfBrowse':} active{:/if:}"
					href="?view=CfBrowse&func=View&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/browse.gif' align="absmiddle">&nbsp;Browse&nbsp;
				</a>
			</li>
			<li {:if VIEW == 'CfStructure':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'CfStructure':} active{:/if:}"
					href="?view=CfStructure&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/structure.gif' align="absmiddle">&nbsp;Structure&nbsp;
				</a>
			</li>
			<li {:if VIEW == 'CfEdit':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'CfEdit':} active{:/if:}"
					href="?view=CfEdit&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/edit.gif' align="absmiddle">&nbsp;Edit&nbsp;
				</a>
			</li>
			<li {:if VIEW == 'Cql':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'Cql':} active{:/if:}"
					href="?view=Cql&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/cql.gif' align="absmiddle">&nbsp;CQL&nbsp;
				</a>
			</li>
			<li {:if VIEW == 'CfSearch':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'CfSearch':} active{:/if:}"
					href="?view=CfSearch&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/search.gif' align="absmiddle">&nbspSearch&nbsp;
				</a>
			</li>
			<li {:if VIEW == 'CfInsert':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'CfInsert':} active{:/if:}"
					href="?view=CfInsert&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					>
					<img src='img/insert.gif' align="absmiddle">&nbsp;Insert&nbsp;
				</a>
			</li>
			<li>
				<a class="tab caution"
					href="?view=CfStructure&func=Truncate&back={:urlencode(REQUEST_URI):}
					&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					onClick="return confirmMsg('Do you really want to :Truncate ColumnFamily ?')";
					>
					<img src='img/empty.gif' align="absmiddle">&nbsp;Empty&nbsp;
				</a>
			</li>
			<li>
				<a class="tab caution"
					href="?view=CfStructure&func=Drop
					&ksName={:KEY_SPACE:}&cfName={:COLUMN_FAMILY:}"
					onClick="return confirmMsg('Do you really want to :Drop ColumnFamily ?')";
					>
					<img src='img/delete.gif' align="absmiddle">&nbsp;Drop&nbsp;
				</a>
			</li>
		</ul>
	</td></tr>
	<tr ><td>&nbsp;</td></tr>
</table>