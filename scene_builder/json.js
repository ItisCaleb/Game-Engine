function output_json(){
    const obj = canvas.getObjects()
    const result = [];
    //console.log(obj)
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
            'x2':coords[2].x - shape.strokeWidth,
            'y2':coords[2].y - shape.strokeWidth
          }
          result.push(rect);
          break
        case 'circle':
          let realRadius = shape.radius * shape.scaleX
          let circle = {
            'type':'circle',
            'x':Number((coords[0].x+realRadius).toFixed(2)),
            'y':Number((coords[0].y+realRadius).toFixed(2)),
            'r':Number(realRadius.toFixed(2)),
          }
          result.push(circle);
          break
        case 'line':
          let x1 = (shape.flipX ^ shape.flipY)?coords[1].x - shape.strokeWidth:coords[0].x;
          let y1 = (shape.flipX ^ shape.flipY)?coords[1].y:coords[0].y;
          let x2 = (shape.flipX ^ shape.flipY)?coords[3].x:coords[2].x;
          let y2 = (shape.flipX ^ shape.flipY)?coords[3].y - shape.strokeWidth:coords[2].y;
          let line = {
            'type':'line',
            'x1':x1,
            'y1':y1,
            'x2':x2,
            'y2':y2
          }
          result.push(line);
          break
      }
    }
    json_output.value = JSON.stringify(result)
  }


document.getElementById('jsonLoader').onchange = (e)=> {
    var reader = new FileReader();
      reader.onload = function (event){
        load_json(reader.result)
      }
      reader.readAsText(e.target.files[0]);
    }
function load_json(json){
    let j = JSON.parse(json)
    console.log(j)
    for(let o of j){
        switch(o.type){
            case 'box':
                let rect = new fabric.Rect({
                    width: 50, height: 50,
                    scaleX: (o.x2-o.x1)/50,
                    scaleY: (o.y2-o.y1)/50,
                    left: o.x1, top: o.y1,
                    fill: null, stroke: 'red',
                    strokeUniform: true
                })
                rect.setControlsVisibility({
                    'mtr':false
                });
                canvas.add(rect)
                break
            case 'circle':
                let circle = new fabric.Circle({
                    radius: 25,
                    left: o.x - o.r, top: o.y-o.r,
                    scaleX: o.r/25,
                    scaleY: o.r/25,
                    fill: null, stroke: 'red',
                    strokeUniform: true
                  })
                  circle.setControlsVisibility({
                    'ml':false,'mb':false,
                    'mr':false,'mt':false,
                    'mtr':false
                  });
                  canvas.add(circle)
                  break
            case 'line':
                let line = new fabric.Line([50, 100, 100, 150], {
                    left: o.x1<o.x2?o.x1:o.x2,
                    top: o.y1<o.y2?o.y1:o.y2,
                    scaleX: (o.x2-o.x1)/50,
                    scaleY: (o.y2-o.y1)/50,
                    stroke: 'red',
                    strokeUniform: true
                })
                canvas.add(line)
        }
    }
    canvas.renderAll();
}