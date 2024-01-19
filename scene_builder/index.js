
let canvas = new fabric.Canvas('canvas')

let oGridGroup;
let image;
let size = 25;
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

json.onclick = output_json

document.addEventListener('keydown', function (event) {
  const key = event.key; // const {key} = event; ES6+
  if (key === "Backspace" || key === "Delete") {
    for (let obj of canvas.getActiveObjects()) {
      canvas.remove(obj);
    }
    return false;
  }
});

function draw_grid(grid_size) {
  canvas.remove(oGridGroup)
  const gridSize = grid_size
  const width = canvas.getWidth();
  const height = canvas.getHeight();
  const lines = [];
  const lineOption = { stroke: 'rgba(0,0,0,1)', strokeWidth: 1, selectable: false };
  for (let i = Math.ceil(width / gridSize); i>=0;i--) {
    lines.push(new fabric.Line([gridSize * i, 0, gridSize * i, height], lineOption));
  }
  for (let i = Math.ceil(height / gridSize); i>=0;i--) {
    lines.push(new fabric.Line([0, gridSize * i, width, gridSize * i], lineOption));
  }
  oGridGroup = new fabric.Group(lines, { left: 0, top: 0 });
  oGridGroup.set('selectable',false)
  oGridGroup.set('hoverCursor','auto')
  canvas.add(oGridGroup);
  canvas.moveTo(oGridGroup, 1);
}

document.getElementById('imgLoader').onchange = function handleImage(e) {
  canvas.remove(oGridGroup)
  var reader = new FileReader();
    reader.onload = function (event){
      var imgObj = new Image();
      imgObj.src = event.target.result;
      imgObj.onload = function () {
        canvas.remove(image)
        image = new fabric.Image(imgObj);
        image.set({
              angle: 0,
              padding: 10,
              cornersize:10,
              opacity: 0.5
        });
        image.set('selectable',false)
        image.set('hoverCursor','auto')
        canvas.setWidth(this.width);
        canvas.setHeight(this.height);
        canvas.centerObject(image);
        canvas.add(image);
        canvas.moveTo(image, 0);
        draw_grid(size);
        canvas.renderAll();
      }
    }
    reader.readAsDataURL(e.target.files[0]);
  }

function toRadians (angle) {
  return angle * (Math.PI / 180);
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

function output_json(){
  canvas.remove(oGridGroup)
  canvas.remove(image)
  const obj = canvas.getObjects()
  const result = [];
  console.log(obj)
  for(let shape of obj){
    if(shape == oGridGroup || shape == image) continue
    let coords = shape.getCoords()
    let gridCoords = oGridGroup.getCoords()
    coords.map((t)=>{
      t.x = Number(((t.x - gridCoords[0].x)/canvas.getZoom()).toFixed(2))
      t.y = Number(((t.y - gridCoords[0].y)/canvas.getZoom()).toFixed(2))
    })
    switch(shape.type){
      case 'rect':
        let rect = {
          'type':'box',
          'x1':coords[0].x,
          'y1':coords[0].y,
          'x2':coords[2].x,
          'y2':coords[2].y
        }
        result.push(rect);
        break
      case 'circle':
        let realRadius = shape.radius * shape.scaleX
        let circle = {
          'type':'circle',
          'x':(coords[0].x+realRadius).toFixed(2),
          'y':(coords[0].y+realRadius).toFixed(2),
          'r':realRadius.toFixed(2),
        }
        result.push(circle);
        break
      case 'line':
        let line = {
          'type':'line',
          'x1':coords[0].x,
          'y1':coords[0].y,
          'x2':coords[2].x,
          'y2':coords[2].y
        }
        result.push(line);
        break
    }
  }
  console.log(result)
  json_output.value = JSON.stringify(result)
  if(image){
    canvas.add(image)
    canvas.moveTo(image, 0);
  }
  draw_grid(size);
}