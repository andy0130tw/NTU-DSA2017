// for sorting two values
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
  { title: 'Trials', data: 'stat', className: 'cell-trial text-right hidden-xs hidden-sm', type: 'dual',
    orderSequence: [ 'desc', 'asc' ],
    render: function(stat, type) {
      if (type == 'sort') {
        return stat ? [ stat.ac_count, -stat.trial_ttl ] : '';
      }
      if (!stat) return '';
      return stat.ac_count + ' / ' + stat.trial_ttl;
    }
  },
  { title: 'Score', data: 'score', className: 'cell-score text-right', orderSequence: [ 'desc', 'asc' ] },
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
    td.classList.add('cell-ac');
  } else if (cellData.verdict == 'WA') {
    td.classList.add('cell-wa');
  } else if (cellData.verdict == 'TLE') {
    td.classList.add('cell-tle');
  }
};

var problemTargets = [];
problems.forEach(function(p, i) {
  var titleArr = [
    '<div class="text-prob-title">' + p.title + '</div>',
    p.user_accepted ?
      '<div class="text-prob-stat">' + p.user_accepted + '/' + p.user_tried + '</div>' : '',
    '<div class="text-prob-team-id">' + (p.pid - 70000) + '</div>'
  ];
  p.title = titleArr.join('');
  p.data = 'result_' + p.pid;
  p.className = 'cell-prob cell-prob-' + p.pid;
  columns.push(p);
  problemTargets.push(i + 4);  // problem starts at 4th column
});

var t = $('#mainTable').DataTable({
  data: data,
  columns: columns,
  columnDefs: [
    {
      targets: problemTargets,
      defaultContent: '<div class="text-status-na">&mdash;</div>',
      render: probRenderFunc,
      createdCell: probMarkFunc,
      orderSequence: [ 'desc' ],
      width: '6rem',
      type: 'dual',
      searchable: false
    }
  ],
  processing: true,
  lengthMenu: [ 16, 25, 32, 50, 100, 200 ],
  order: [[ 3, 'desc' ]],
  scrollX: true,
  scrollY: '65vh',
  fixedColumns: {
    leftColumns: 4
  },
  colReorder: {
    fixedColumnsLeft: 4
  },
});

t.on('order.dt search.dt', function () {
  t.column(0, { search: 'applied', order: 'applied' })
    .nodes()
    .each(function(cell, i) {
      cell.innerHTML = i + 1;
    });
}).draw();

(function(b,c){var $=b.jQuery||b.Cowboy||(b.Cowboy={}),a;$.throttle=a=function(e,f,j,i){var h,d=0;if(typeof f!=="boolean"){i=j;j=f;f=c}function g(){var o=this,m=+new Date()-d,n=arguments;function l(){d=+new Date();j.apply(o,n)}function k(){h=c}if(i&&!h){l()}h&&clearTimeout(h);if(i===c&&m>e){l()}else{if(f!==true){h=setTimeout(i?k:l,i===c?e-m:e)}}}if($.guid){g.guid=j.guid=j.guid||$.guid++}return g};$.debounce=function(d,e,f){return f===c?a(d,e,false):a(d,f,e!==false)}})(this);

$(window)
  .off('resize')
  .on('resize', $.debounce(200, function() {
    t.columns.adjust();
  }));
