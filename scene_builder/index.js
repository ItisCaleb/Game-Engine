
let canvas = new fabric.Canvas('canvas')

let oGridGroup;
let image;
let size = 25;
let gridWidth = canvas.getWidth()
let gridHeight = canvas.getHeight();
draw_grid(size);

canvas.on('mouse:wheel', function (opt) {
  var delta = opt.e.deltaY;
  var zoom = canvas.getZoom();
  zoom *= 0.999 ** delta;
  if (zoom > 20) zoom = 20;
  if (zoom < 0.01) zoom = 0.01;
  canvas.zoomToPoint({ x: opt.e.offsetX, y: opt.e.offsetY }, zoom);
  opt.e.preventDefault();
  opt.e.stopPropagation();
  draw_grid(size)
});

canvas.on('mouse:down', function (opt) {
  var evt = opt.e;
  if (evt.altKey === true) {
    this.isDragging = true;
    this.selection = false;
    this.lastPosX = evt.clientX;
    this.lastPosY = evt.clientY;
  }
});
canvas.on('mouse:move', function (opt) {
  if (this.isDragging) {
    var e = opt.e;
    var vpt = this.viewportTransform;
    vpt[4] += e.clientX - this.lastPosX;
    vpt[5] += e.clientY - this.lastPosY;
    this.requestRenderAll();
    this.lastPosX = e.clientX;
    this.lastPosY = e.clientY;
  }
});
canvas.on('mouse:up', function (opt) {
  // on mouse up we want to recalculate new interaction
  // for all objects, so we call setViewportTransform
  this.setViewportTransform(this.viewportTransform);
  this.isDragging = false;
  this.selection = true;
});

canvas.on('object:added', output_json);
canvas.on('object:removed', output_json);



box.onclick = () => {
  const rect = new fabric.Rect({
    width: 50, height: 50,
    left: 100, top: 100,
    fill: null, stroke: 'red',
    strokeUniform: true
  })
  rect.setControlsVisibility({
    'mtr':false
  });
  canvas.add(rect)
}

circle.onclick = () => {
  const circle = new fabric.Circle({
    radius: 25,
    left: 100, top: 100,
    fill: null, stroke: 'red',
    strokeUniform: true
  })
  circle.setControlsVisibility({
    'ml':false,'mb':false,
    'mr':false,'mt':false,
    'mtr':false
  });
  canvas.add(circle)
}

line.onclick = () => {
  const line = new fabric.Line([50, 100, 100, 150], {
    left: 170,
    top: 150,
    stroke: 'red',
    strokeUniform: true
  })
  canvas.add(line)
}



let _copied = null;
document.addEventListener('keydown', function (event) {
  const key = event.key; // const {key} = event; ES6+
  if (key === "Delete") {
    for (let obj of canvas.getActiveObjects()) {
      canvas.remove(obj);
    }
    return false;
  }
  if (event.ctrlKey && (key === 'c' || key === 'C')) {
      canvas.getActiveObject().clone(cloned=> {
        _copied = cloned;
      });
      return false
  }
  if (event.ctrlKey && (event.key === 'v' || event.key === 'V')) {
    _copied.clone(clonedObj=>{
      canvas.discardActiveObject();
      clonedObj.set({
        left: clonedObj.left + 10,
        top: clonedObj.top + 10,
        evented: true,
      });
      if (clonedObj.type === 'activeSelection') {
        // active selection needs a reference to the canvas.
        clonedObj.canvas = canvas;
        clonedObj.forEachObject(function(obj) {
          canvas.add(obj);
        });
        // this should solve the unselectability
        clonedObj.setCoords();
      } else {
        canvas.add(clonedObj);
      }
      _copied.top += 10;
      _copied.left += 10;
      canvas.setActiveObject(clonedObj);
      canvas.requestRenderAll();
    });
    return false;
  }
});

function draw_grid(grid_size) {
  canvas.remove(oGridGroup)
  const gridSize = grid_size
  const width = gridWidth
  const height = gridHeight
  const lines = [];
  const lineOption = { stroke: 'rgba(0,0,0,1)', strokeWidth: 1, selectable: false };
  let w = Math.ceil(width / gridSize)
  let h = Math.ceil(height / gridSize)
  for (let i = w; i>=0;i--) {
    lines.push(new fabric.Line([gridSize * i, 0, gridSize * i, h*gridSize], lineOption));
  }
  for (let i = h; i>=0;i--) {
    lines.push(new fabric.Line([0, gridSize * i, w*gridSize, gridSize * i], lineOption));
  }
  oGridGroup = new fabric.Group(lines, { left: 0, top: 0 });
  oGridGroup.set('selectable',false)
  oGridGroup.set('hoverCursor','auto')
  canvas.add(oGridGroup);
  if(image) canvas.moveTo(oGridGroup, 1);
  else canvas.moveTo(oGridGroup, 0);
}

document.getElementById('imgLoader').onchange = function handleImage(e) {
  canvas.remove(oGridGroup)
  var reader = new FileReader();
    reader.onload = function (event){
      var imgObj = new Image();
      imgObj.src = event.target.result;
      imgObj.onload = function () {
        canvas.remove(image)
        image = new fabric.Image(imgObj,{
            opacity: 0.5,
        })
        document.querySelector('#image-size').innerHTML = `Image Size: ${this.width}*${this.height}`
        gridWidth = this.width;
        gridHeight = this.height;
        image.set('selectable',false)
        image.set('hoverCursor','auto')
        canvas.add(image);
        canvas.moveTo(image, 0);
        draw_grid(size);
        canvas.renderAll();
      }
    }
    reader.readAsDataURL(e.target.files[0]);
  }


function copyToClipboard() {
  // Get the text field
  var copyText = json_output

  // Select the text field
  copyText.select();
  copyText.setSelectionRange(0, 99999); // For mobile devices

   // Copy the text inside the text field
  navigator.clipboard.writeText(copyText.value);

}

