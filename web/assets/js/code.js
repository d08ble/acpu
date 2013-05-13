(function(){
  var root = this;
  var _ = root._;
  var acpulCode;

  acpulCode = root.acpulCode = {};

  //
  // UTIL: OUT PREFIX
  //

  function outPrefix(prefix)
  {
    var i, s = "";
    for (i = 0; i < prefix; i++)
    {
      s = s + "&nbsp";
    }
    return s;
  }

  //
  // STREE: DEFAULT OUTPUT
  //

  function processStreeNode(prefix, obj)
  {
    var items = [];
    var type = obj.type;
//    items.push();
    var sval = '<div class=acpul-stype-val' + ' id=' +obj.id+ '>' +obj.val+ '</div>';
    items.push('<div class=acpul-stype-' +type+ '>' +outPrefix(prefix)+type+ '&nbsp'+'</div>' +sval);
    $.each(obj.nodes, function(key, val) {
      items = items.concat(processStreeNode(prefix+1, val));
    });

    return items;
  }

  function processStree(prefix, obj)
  {
    return processStreeNode(prefix, obj);
  }

  function processFileMap(data)
  {
    var items = [];

    $.each(data, function(key, val) {
      if (!val)
      {
        items.push('<div id="' +key+ '">' +key+ ' ' +val+ '</div>');
      }
      else
      {
        items.push('<div id="' +key+ '">' +key+ '</br>{' +processStree(0, val['stree']).join('')+ '};</div>');
      }
    });

    return items;
  };

  //
  // STREE: SOURCE OUTPUT
  //

  var sources = {};

  function loadSourcesForFileMap(data, complete)
  {
    var count = Object.keys(data).length;

    $.each(data, function(key, val) {
      if (val)
      {
        $.get('code/sources/' +key+ '.acpul', function(data) {
          console.log(key, data);
          sources[key] = data;
          count--;
          if (count == 0)
          {
            complete(sources);
          }
        });
      }
      else
      {
        count--;
      }
    })
  };

  //
  // STREAM
  //

  var Stream = {};

  Stream.peek = function(is, offset) {
    if (is.pos > offset)
      return "";
    var size = offset - is.pos;
    var s = is.data.substr(is.pos, size);
    is.pos = offset;
    return s;
  };

  Stream.read = function(is, size) {
    var s = is.data.substr(is.pos, size);
    is.pos += size;
    return s;
  };

  Stream.write = function(stream, data) {
/*    var ss = data.split('\n');
    for (var i = 0; i < ss.length; i++)
    {
      stream.data = stream.data + ss[i];
      if (i != ss.length-1)
        stream.data += '</br>';
    }
    if (data[data.length-1] == '\n')
        stream.data += '</br>'; */
    stream.data += data;
  };

  function buildSourceProcessNode(os, is, obj)
  {
    var offset = parseInt(obj.pos);
    var s = Stream.peek(is, offset);

    function store(os, s, type, id)
    {
      Stream.write(os, '<div class=acpul-stype-' +type+ (id > 0 ? ' id=' +id : '') + '>');

      for (var i = 0; i < s.length; i++)
      {
        var c = s[i];
        if (c == ' ')
        {
          Stream.write(os, '&nbsp');
        }
        else if (c == '\n')
        {
          Stream.write(os, '</div></br>');
          Stream.write(os, '<div class=acpul-stype-' +type+ (id > 0 ? ' id=' +id : '') + '>');
  //        Stream.write(os, c);
        }
        else
        {
          Stream.write(os, c);
        }
      }        

      Stream.write(os, '</div>');
    }

    if (s != '')
      store(os, s, 'undefined', -1);

    s = Stream.peek(is, offset+obj.val.length);

    store(os, s, obj.type, obj.id);

//    var size = obj.type;
//    s = Stream.read(is, offset, size);
//    Stream.write(os, s);
    
    $.each(obj.nodes, function(key, val) {
      buildSourceProcessNode(os, is, val);
//      if (obj.type)
//      Stream.write(os, s);
    });


//    s = Stream.read(is, size);
  }

  function buildSource(tree, key, source)
  {
    var is = {};
    is.pos = 0;
    is.data = source;

    var os = {}
    os.pos = 0;
    os.data = "";

    tree.pos = "0";

    Stream.write(os, '</br></br>' +key+ ' --------</br></br>');

    buildSourceProcessNode(os, is, tree);

    console.log(os.data);

    return os.data;
  }

  function buildSources(data, sources)
  {
    $.each(data, function(key, val) {
      var source = sources[key];

      if (source !== undefined)
      {
        var html = buildSource(val['stree'], key, source);
        $('<div/>', {
          'class': 'acpul-fileMap-files',
          html: html
        }).appendTo('body');
      }
    });
  };

//  function processFiles()

//  $.getJSON('code/_test0000.json', function(data) {
  $.getJSON('code/core.json', function(data) {
    debugger;

    var items = processFileMap(data);


    var a = $('<div/>');
    $('<div/>', {
      'class': 'acpul-fileMap-files',
      html: items.join('')
    }).appendTo('body');

    loadSourcesForFileMap(data, function(sources)
    {
      buildSources(data, sources)
    });
  });

}).call(this);
