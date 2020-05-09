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