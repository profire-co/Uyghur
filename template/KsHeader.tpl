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
	</th></tr>
	<tr><td colspan='10'>
		<ul id="tablist">
			<li {:if VIEW == 'KsStructure':}class="current"{:/if:}>
				<a class="tab{:if VIEW == 'KsStructure':} active{:/if:}"
					href="?view=KsStructure&ksName={:KEY_SPACE:}"
					>
					<img src='img/keyspace.gif' align="absmiddle">&nbsp;Structure&nbsp;
				</a>
			</li>
			<li>
				<a class="tab caution"
					href="?view=KsStructure&func=Drop&ksName={:KEY_SPACE:}"
					onClick="return confirmMsg('Do you really want to :Drop KeySpace ?')";
					>
					<img src='img/delete.gif' align="absmiddle">&nbsp;Drop&nbsp;
				</a>
			</li>
		</ul>
	</td></tr>
	<tr ><td>&nbsp;</td></tr>
</table>