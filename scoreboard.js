jQuery.extend(jQuery.fn.dataTableExt.oSort, {
  "dual-asc": function(x, y) {
    if (!x) return -1;
    if (!y) return 1;
    if (x[0] != y[0])
        return x[0] - y[0];
    return x[1] - y[1];
  },

  "dual-desc": function(x, y) {
    if (!x) return 1;
    if (!y) return -1;
    if (x[0] != y[0])
        return y[0] - x[0];
    return y[1] - x[1];
  }
});

var columns = [
  { title: '#', defaultContent: '', className: 'cell-rank text-right', orderable: false, searchable: false },
  { title: 'User', data: 'username', className: 'cell-user' },
  { title: 'Trials', data: 'stat', className: 'cell-trial text-right', type: 'dual', orderSequence: [ 'desc', 'asc' ],
    render: function(stat, type) {
      if (type == 'sort') {
        return stat ? [ stat.ac_count, -stat.trial_ttl ] : '';
      }
      if (!stat) return '';
      return stat.ac_count + ' / ' + stat.trial_ttl;
    }
  },
  { title: 'Score', data: 'score', className: 'cell-score text-right' },
];

var probRenderFunc = function(obj, type) {
  if (type == 'sort') {
    return obj ? [ obj.best.scr, -obj.trial ] : '';
  }
  if (!obj) return;
  var desc = obj.verdict + ' ' + obj.best.scr;
  if (obj.verdict == 'AC') desc = 'AC';
  return desc + ' / ' + obj.trial;
};

var probMarkFunc = function(td, cellData, rowData, row, col) {
  if (cellData.verdict == 'AC') {
    $(td).addClass('cell-ac');
  } else if (cellData.verdict == 'WA') {
    $(td).addClass('cell-wa');
  } else if (cellData.verdict == 'TLE') {
    $(td).addClass('cell-tle');
  }
};

problems.forEach(function(p) {
  p.data = 'result_' + p.pid;
  p.className = 'cell-prob cell-brob-' + p.pid;
  p.defaultContent = '<span class="text-status-na">--</span>';
  p.render = probRenderFunc;
  p.type = 'dual';
  p.orderSequence = [ 'desc' ];
  p.width = '8rem';
  p.createdCell = probMarkFunc;
  columns.push(p);
});

var t = $('#mainTable').DataTable({
  data: data,
  columns: columns,
  columnDefs: [

  ],
  // paging: false,
  processing: true,
  lengthMenu: [ 15, 25, 50, 100 ],
  order: [[ 3, 'desc' ]],
  scrollX: true,
  scrollY: '70vh',
  fixedColumns: {
    leftColumns: 4
  },
  deferRender: true
});

t.on('order.dt search.dt', function () {
  t.column(0, { search:'applied', order:'applied' })
    .nodes()
    .each(function(cell, i) {
      cell.innerHTML = i + 1;
    });
}).draw();
