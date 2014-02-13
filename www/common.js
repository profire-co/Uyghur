/*
 * Uyghur
 * Copyright (c) 2014 Profire Co.,Ltd
 *
 * This software is released under the MIT License.
 *
 * http://opensource.org/licenses/mit-license.php
 */

function ColorMouseIn(obj,table_id){
    var color  = "#CCFFCC";
    var t_obj=document.getElementById(table_id);
    if ( obj.rowIndex != undefined ) {
        //tr処理
        for ( var i = 0; i < t_obj.rows[obj.rowIndex].cells.length;i++ ) {
            t_obj.rows[obj.rowIndex].cells[i].style.backgroundColor = color;
        }
    }
}

function ColorMouseOut(obj,table_id){
    var color = 'white';
    var t_obj=document.getElementById(table_id);
    if ( obj.rowIndex != undefined ) {
        //tr処理
        for ( var i = 0; i < t_obj.rows[obj.rowIndex].cells.length;i++ ) {
            switch(t_obj.rows[obj.rowIndex].className){
              default:
                color = 'white';
                break;
              case 'bg_list1':
                color = '#E5E5E5';
                break;
              case 'bg_list2':
                color = '#D5D5D5';
                break;
            }
            t_obj.rows[obj.rowIndex].cells[i].style.backgroundColor = color;
        }
    }
}

function CfInsertAddColumn(id,num) {
    var table    = document.getElementById(id);
    var last_row = table.rows.length-1;
    var row      = table.insertRow(last_row);
    var cell1    = row.insertCell(0);
    var cell2    = row.insertCell(1);
    var cell3    = row.insertCell(2);
    var className = ((last_row-2)%2 == 0)?"bg_list1":"bg_list2";
    row.setAttribute("class",className);
    row.className = className;
    var columns = '<input type="text" name="columns['+num+'][]" size="30">';
    var values  = '<input type="text" name="values['+num+'][]"  size="30">';
    var nulls   = '&nbsp;';
    cell1.innerHTML = columns;
    cell2.innerHTML = values;
    cell3.innerHTML = nulls;
}

function CfInsertAddRow(id,list) {
    var table     = document.getElementById(id);
    var last_row  = table.rows.length-1;
    var table_num = table.rows.length-3;
    var row       = table.insertRow(last_row-1);
    var cell      = row.insertCell(0);
    var HTML = [
        '<table cellspacing=5  cellpadding=5 border=0 id="updateList_'+table_num+'">',
        '	<tr>',
        '		<th class="bg_menu">RowKey</th>',
        '		<td colspan="2"><input type="text" name="rowKeys[]"  size="40"></td>',
        '	</tr>',
        '	<tr class="bg_menu">',
        '		<th width="160">Column Name</th>',
        '		<th width="160">Column Value</th>',
        '		<th>NULL</th>',
        '	</tr>',
        ].join('\n');
    if( list.length > 0 ) {
        for( var i in list ) {
            HTML += [
                '	<tr class="bg_list1">',
                '		<td><input type="text" name="columns['+table_num+'][]" value="'+list[i]+'" size="30"></td>',
                '		<td><input type="text" name="values['+table_num+'][]" size="30"></td>',
                '		<td>&nbsp;</td>',
                '	</tr>',
                ].join('\n');
        }
    }
    else {
    HTML += [
        '	<tr class="bg_list1">',
        '		<td><input type="text" name="columns['+table_num+'][]"size="30"></td>',
        '		<td><input type="text" name="values['+table_num+'][]" size="30"></td>',
        '		<td>&nbsp;</td>',
        '	</tr>',
        ].join('\n');
    }
    HTML += [
        '	<tr><td>',
        '		<input type="button" value="AddColumn" onClick="CfInsertAddColumn(\'updateList_'+table_num+'\',\''+table_num+'\');" /> ',
        '	</td></tr>',
        '</table>',
        ].join('\n');
    //console.log(HTML);
    cell.innerHTML = HTML;
    
}

function CfSearchAddRowky(id) {
    var table     = document.getElementById(id);
    var last_row  = table.rows.length;
    var row       = table.insertRow(last_row);
    var cell1     = document.createElement("th");
    
    cell1.setAttribute("class","bg_menu");
    cell1.className = "bg_menu";
    row.appendChild(cell1);
    
    
    var label   = 'RowKey';
    var input   = '<input type="text" name="rowKeys[]"  size=\'40\'>';
    cell1.innerHTML = label;
    var cell2     = row.insertCell(1);
    
    cell2.innerHTML = input
    
}

function confirmMsg(msg) {
    if( confirm(msg) ) {
        return true;
    }
    else {
        return false;
    }
}
