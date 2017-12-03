<template>
  <div class="container-fluid backgr">
    <div class="row justify-content-end" style="margin-bottom: 10vh;">
      <div class="col-lg-1">
        <img class="img-fluid" src="./assets/uminho.png">
      </div>
    </div>
    <div class="row justify-content-center" style="margin-bottom: 3vh;">
      <h2 class="title">SNMP Manager</h2>
    </div>
    <div class="row justify-content-center">
      <div class="col-lg-4">
        <div class="content card">
          <h5 class="card-header text-center">Partitions</h5>
          <div class="card-body">
            <table class="table table-hover table-dark">
              <thead>
                <tr>
                  <th scope="col">#</th>
                  <th scope="col">Description</th>
                  <th scope="col">Size_GB</th>
                  <th scope="col">Used_GB</th>
                  <th scope="col">Free_%</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="(partition, index) in partitions" :key="index">
                  <th scope="row">{{ index + 1}}</th>
                  <td class="text-center">{{ partition.desc }}</td>
                  <td class="text-center">{{ partition.size }}</td>
                  <td :id="'used'+partition.desc" class="text-center">{{ partition.used }}</td>
                  <td v-if="parseInt(partition.free) > 60" :id="'free'+partition.desc" class="text-center" style="color: green">{{ partition.free }}</td>
                  <td v-if="parseInt(partition.free) <= 60 && parseInt(partition.free) >= 30" :id="'free'+partition.desc" class="text-center" style="color: yellow">{{ partition.free }}</td>
                  <td v-if="parseInt(partition.free) < 30" :id="'free'+partition.desc" class="text-center" style="color: red">{{ partition.free }}</td>                                
                </tr>
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
    <footer class="footer">
      <div class="footer_list">
        <a href="https://github.com/joao-coelho" target="_blank">
          <img src="./assets/github.png" class="github">
        </a>
        <span class="name">João Coelho</span>
      </div>
    </footer>
  </div>
</template>

<script>
  var ws = new WebSocket("ws://localhost:8081");
  
  export default {
    name: "app",
    data() {
      return {
        partitions: []
      }
    },
    methods: {
      findPart(obj){
        for (var i=0; i < this.partitions.length; i++) {
          if (this.partitions[i].desc == obj.desc)
            return true;
        }
        return false;
      },
      updatePart(obj){
        var index = this.findElement(obj.desc);
        if (this.partitions[index].used != obj.used) {
          this.flashCell("used"+obj.desc);
        }
        if (this.partitions[index].free != obj.free) {
          this.flashCell("free"+obj.desc);
        }
        this.partitions[obj.desc] = obj;
      },
      findElement(desc){
        for(var i=0; i < this.partitions.length; i++)
          if (this.partitions[i].desc == desc)
            return i;
      },
      flashCell(id){
        var elem = document.getElementById(id);
        elem.classList.add('flash');
        elem.addEventListener('animationend', () => {
          elem.classList.remove("flash");
        });      
      }
    },
    mounted() {
      const vm = this;
      // Connection opened
      ws.addEventListener('open', function (event) {
          ws.send('Starting SNMP Manager application...');
         console.log('Starting SNMP Manager application...');
      });
      // Listen for messages
      ws.addEventListener('message', function (event) {
          var jsonObj = JSON.parse(event.data);
          console.log('Message from server: ', jsonObj);
          if (vm.findPart(jsonObj)){
            vm.updatePart(jsonObj);
          }
          else {
            vm.partitions.push(jsonObj);
          }
      });
    }
  }
</script>

<style scoped>
  .full-width {
    width: 100%;
  }
  .backgr {
    padding: 0;
    margin: 0;
    height: 100vh;
    width: 100vw;
    z-index: -1;
    background: url(./assets/prometeu.jpg) no-repeat center center fixed;
    background-size: cover;
    position: absolute;
    overflow: hidden;
  }
  .content {
    background: linear-gradient(to top left, rgba(112, 25, 25, 0.6), rgba(134, 0, 0, 0.6)), url(./assets/prometeu.jpg) no-repeat center center fixed;
      background-size: cover;
    box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.8), 0 6px 21px 0 rgba(0, 0, 0, 0.4);
  }
  .title {
    color: white;
    font-style: italic;
  }

  .footer {
    position: absolute;
    bottom: 0;
    width: 100%;
    height: 45px;
    line-height: 45px;
    background-color: #212121;
    opacity: 0.9;
  }

  .name {
    color: lightgrey;
  }

  .footer_list {
    display: inline-block;
    float: right;
    margin-right: 2vw;
  }

  .github {
    margin-right: 0.5vw;
    height: 25px;
    width: 25px;
  }

  @-webkit-keyframes flash {
    from { background-color: lightgrey; }
    to { background-color: inherit; }
  }
  @-moz-keyframes flash {
    from { background-color: lightgrey; }
    to { background-color: inherit; }
  }
  @-o-keyframes flash {
    from { background-color: lightgrey; }
    to { background-color: inherit; }
  }
  @keyframes flash {
    from { background-color: lightgrey; }
    to { background-color: inherit; }
  }
  .flash {
    -webkit-animation: flash 0.7s 1; /* Safari 4+ */
    -moz-animation:    flash 0.7s 1; /* Fx 5+ */
    -o-animation:      flash 0.7s 1; /* Opera 12+ */
    animation:         flash 0.7s 1; /* IE 10+ */
  }
</style>
