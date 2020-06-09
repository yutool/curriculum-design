/**
 * 首页
 * @author ankoye
 */

// 获取当前登录用户，并设置导航栏
window.onload = function () {
  const result = getCurrentUser()
  const currentUser = document.getElementById('currentUser')
  const loginNav = document.getElementById('loginNav')
  const logoutNav = document.getElementById('logoutNav')
  if (result.data) {
    currentUser.innerHTML = result.data.username
    loginNav.classList.add('hidden')
    logoutNav.classList.remove('hidden')
  } else {
    currentUser.innerHTML = ''
    loginNav.classList.remove('hidden')
    logoutNav.classList.add('hidden')
  }
}
// 退出
function logout() {
  removeCurrentUser()
  location.href = 'login.html'
}

// 添加购物车
function processAddCart(goods) {
  const { data } = getCurrentUser()
  const shadeHint = document.getElementById('goods_' + goods.id)
  if (data) {
    addCart(goods)
    // 添加提示
    const node = document.createElement("p")
    node.innerHTML = '购物车+1'
    shadeHint.appendChild(node)
    const timer = setInterval(() => {
      shadeHint.removeChild(node)
      window.clearInterval(timer)
    }, 1000)
  } else {
    location.href = 'login.html'
  }
}

// ---------- 女装 ----------
const DomWPT = document.getElementById('womens-product-title')
const DomWP = document.getElementById('womens-product')
var womensWearTitle = {
  title: '女装',
  items: [
    { link: '#', value: '时尚套装' },
    { link: '#', value: 'T恤' },
    { link: '#', value: '卫衣' },
    { link: '#', value: '连衣裙' },
    { link: '#', value: '牛仔裤' },
    { link: '#', value: '休闲裤' },
  ],
  btnId: 'a'
}
DomWPT.innerHTML = template('t-product-title', womensWearTitle)

var womensWear = {
  start: 0, // 开始下标
  size: 12, // 需要多少条
  products: [
    { id: '1', image: '../img/woman/1.webp', title: '2020夏季设计感日系气质短袖', price: '62' },
    { id: '2', image: '../img/woman/2.webp', title: '夏装时尚韩版网红气质休闲装', price: '71' },
    { id: '3', image: '../img/woman/3.webp', title: '夏季流行气质短袖潮牌', price: '90' },
    { id: '4', image: '../img/woman/4.webp', title: '夏季时尚韩版遮肚网红T恤', price: '50' },
    { id: '5', image: '../img/woman/5.webp', title: '2020夏季新款修身显瘦短袖', price: '56' },
    { id: '6', image: '../img/woman/6.webp', title: '春季新款长袖学生上衣百搭', price: '106' },
    { id: '7', image: '../img/woman/7.webp', title: '春季冰丝短袖T恤修身露锁骨', price: '66' },
    { id: '8', image: '../img/woman/8.webp', title: '网红港风ins衬衫百搭蹦迪上衣', price: '70' },
    { id: '9', image: '../img/woman/9.webp', title: '两件套吊带蕾丝衬衫宽松', price: '59' },
    { id: '10', image: '../img/woman/10.webp', title: '春季新款超火韩版长袖ins潮', price: '51' },
    { id: '11', image: '../img/woman/11.webp', title: '夏2020新款韩版纯色长袖学生', price: '60' },
    { id: '12', image: '../img/woman/12.webp', title: '网红短袖T恤2020新款韩版宽松', price: '49' },
    { id: '13', image: '../img/woman/13.webp', title: '2020春夏新款时尚短袖网红上衣', price: '90' },
    { id: '14', image: '../img/woman/14.webp', title: '春夏季新款短袖T恤纯棉上衣', price: '102' },
    { id: '15', image: '../img/woman/15.webp', title: '超火短袖T恤女夏韩版韩气ins潮', price: '109' },
    { id: '16', image: '../img/woman/16.webp', title: '春季新款超仙上衣衬衫女设计感', price: '49' },
    { id: '17', image: '../img/woman/17.webp', title: '短袖T恤新款少女宽松日系小清新', price: '50' },
    { id: '18', image: '../img/woman/18.webp', title: '超火短袖T恤韩版bf百搭ins潮', price: '62' },
    { id: '19', image: '../img/woman/19.webp', title: '夏季2020新款韩版宽松短袖女', price: '92' },
    { id: '20', image: '../img/woman/20.webp', title: '简约学生网红休闲气质短袖', price: '79' }
  ]
}
DomWP.innerHTML = template('t-product', womensWear)
// 换一批按钮
document.getElementById(womensWearTitle.btnId).onclick = function () {
  womensWear.start = (womensWear.start + womensWear.size) % womensWear.products.length
  DomWP.innerHTML = template('t-product', womensWear)
}

// ---------- 男装 ----------
const DomMWT = document.getElementById('mens-product-title')
const DomMW = document.getElementById('mens-product')
var mensWearTitle = {
  title: '男装',
  items: [
    { link: '#', value: '当季新品' },
    { link: '#', value: '万能衬衫' },
    { link: '#', value: '百搭休闲裤' },
    { link: '#', value: '潮鞋' },
  ],
  btnId: 'b'
}
DomMWT.innerHTML = template('t-product-title', mensWearTitle)

var mensWear = {
  start: 0,
  size: 12,
  products: [
    { id: '21', image: '../img/man/1.webp', title: '夏季男装短袖男新款潮流帅气', price: '69' },
    { id: '22', image: '../img/man/2.webp', title: '男士T恤短袖圆领韩版修身时尚', price: '49' },
    { id: '23', image: '../img/man/3.webp', title: '夏季新款圆领短袖纯棉宽松', price: '50' },
    { id: '24', image: '../img/man/4.webp', title: '2020夏季纯棉打底短袖ins潮流', price: '70' },
    { id: '25', image: '../img/man/5.webp', title: '纯棉夏季新款男士百搭休闲短袖', price: '109' },
    { id: '26', image: '../img/man/6.webp', title: '夏季男士印花T恤男复古潮牌', price: '62' },
    { id: '27', image: '../img/man/7.webp', title: '夏季短袖T恤男薄款纯棉圆领打底', price: '96' },
    { id: '28', image: '../img/man/8.webp', title: '春夏长袖男撞色拼接青少年百搭', price: '39' },
    { id: '29', image: '../img/man/9.webp', title: '港风夏季男装休闲T恤圆领修身', price: '56' },
    { id: '30', image: '../img/man/10.webp', title: '春夏修长袖衬衫2019男潮牌', price: '92' },
    { id: '31', image: '../img/man/11.webp', title: '港风夏季潮牌原宿风短袖宽松', price: '106' },
    { id: '32', image: '../img/man/12.webp', title: '2020夏季新款情侣装短袖T恤', price: '52' },
    { id: '33', image: '../img/man/13.webp', title: '港风夏季男装潮流短袖圆领纯棉', price: '49' },
    { id: '34', image: '../img/man/14.webp', title: '港风ins小猫印花短袖T恤学生男', price: '70' },
    { id: '35', image: '../img/man/15.webp', title: '港风ins外套男春秋款宽松拼色上衣', price: '98' },
    { id: '36', image: '../img/man/16.webp', title: '2020夏季新款半袖宽松帅气搭配', price: '62' },
    { id: '37', image: '../img/man/17.webp', title: '男士夏季新款圆领潮流纯棉上衣', price: '102' },
    { id: '38', image: '../img/man/18.webp', title: '夏季新款男士潮流印花运动背心', price: '62' },
    { id: '39', image: '../img/man/19.webp', title: '港风夏季男装新款休闲潮流上衣', price: '50' },
    { id: '40', image: '../img/man/20.webp', title: '夏季短袖T恤2019纯棉ins男装', price: '60' }
  ]
}
DomMW.innerHTML = template('t-product', mensWear)

// 换一批按钮
document.getElementById(mensWearTitle.btnId).onclick = function () {
  mensWear.start = (mensWear.start + mensWear.size) % mensWear.products.length
  DomMW.innerHTML = template('t-product', mensWear)
}

/**
 * 控制首页图片轮播
 * @author ankoye
 */

// 获取所有图片项
const images = document.querySelectorAll('.carousel-item')
// 小圆点
const lis = document.querySelectorAll('.carousel-li')
// 上下页按钮
const btns = document.querySelectorAll('.carousel-control')
var timer       // 定时器对象
var index = 0   // 当前显示图片下标

// 切换图片函数
function switchImage(tmp = null) {
  for (let i = 0; i < images.length; i++) {
    images[i].classList.remove('active')
    lis[index].style.background = ''
  }
  if (tmp === null) {
    index = (index + 1) % images.length
  } else {
    index = tmp
  }
  images[index].classList.add('active')
  lis[index].style.background = '#fff'
}

// 开启定时器
timer = setInterval(switchImage, 3000)

// 绑定小圆点点击效果
for (let i = 0; i < lis.length; i++) {
  lis[i].onclick = function () {
    window.clearInterval(timer)
    switchImage(i)
    timer = setInterval(switchImage, 3000)
  }
}

// 绑定上下页点击效果
btns[0].onclick = function () {
  window.clearInterval(timer)
  switchImage((index + images.length - 1) % images.length)
  timer = setInterval(switchImage, 3000)
}
btns[1].onclick = function () {
  window.clearInterval(timer)
  switchImage((index + 1) % images.length)
  timer = setInterval(switchImage, 3000)
}

// 鼠标悬停图片暂停轮播
for (let i = 0; i < images.length; i++) {
  images[i].onmouseover = function () {
    window.clearInterval(timer)
  }
  images[i].onmouseout = function () {
    if (timer) window.clearInterval(timer)
    timer = setInterval(switchImage, 3000)
  }
}