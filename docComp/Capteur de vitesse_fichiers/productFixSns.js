(window["webpackJsonp-ae-fe/detail-ui-2"]=window["webpackJsonp-ae-fe/detail-ui-2"]||[]).push([[5],{612:function(e,t,n){},758:function(e,t,n){"use strict";n.r(t);var a=n(3),o=n.n(a),i=n(4),r=n.n(i),s=n(5),c=n.n(s),l=n(6),u=n.n(l),p=n(0),h=n(95),m=new(function(){function e(){o()(this,e),this.shareTitle=this.getMetaContent('meta[property="og:title"]')||this.getMetaContent('meta[name="keywords"]'),this.shareImage=this.getMetaContent('meta[property="og:image"]'),this.shareContent=this.getMetaContent('meta[name="description"]')||this.shareTitle,this.processUrl=this.geteProcessUrl()}return r()(e,[{key:"geteProcessUrl",value:function(){var e,t=window.location.href;return/([^?]+)(?:\?.+)/.test(t)&&(e=t.match(/([^?]+)(?:\?.+)/)),t=e&&e.length>1?e[1]:t}},{key:"getFBUrl",value:function(){var e=this.getShareUrl("facebook"),t="https://www.facebook.com/sharer/sharer.php";return t+="?u=".concat(encodeURIComponent(e))}},{key:"getVKUrl",value:function(){var e=this.getShareUrl("vk"),t="https://vk.com/share.php";return t+="?url=".concat(encodeURIComponent(e),"&title=").concat(encodeURIComponent(this.shareTitle),"&image=").concat(encodeURIComponent(this.shareImage))}},{key:"getTwinnerUrl",value:function(){var e=this.getShareUrl("twinner"),t="https://twitter.com/intent/tweet",n=this.shareTitle.length>110?"".concat(this.shareTitle.substring(0,110),"..."):this.shareTitle;return t+="?url=".concat(encodeURIComponent(e),"&text=").concat(encodeURIComponent(n))}},{key:"getPinterestUrl",value:function(){var e=this.getShareUrl("pinterest"),t="https://www.pinterest.com/pin/create/button/";return t+="?url=".concat(encodeURIComponent(e),"&media=").concat(encodeURIComponent(this.shareImage),"&description=").concat(encodeURIComponent(this.shareContent))}},{key:"getGoogleUrl",value:function(){var e=this.getShareUrl("google"),t="https://plus.google.com/share";return t+="?url=".concat(encodeURIComponent(e))}},{key:"getOKUrl",value:function(){var e=this.getShareUrl("ok"),t="https://connect.ok.ru/offer";return t+="?url=".concat(encodeURIComponent(e),"&title=").concat(encodeURIComponent(this.shareTitle),"&imageUrl=").concat(encodeURIComponent(this.shareImage))}},{key:"getShareUrl",value:function(e){var t=this.processUrl;return/\?[^.&=]+=[^.&=]/.test(t)?"".concat(t,"&channel=").concat(e):"".concat(t,"?channel=").concat(e)}},{key:"getMetaContent",value:function(e){var t=document.querySelector(e);if(t)return t.content}}]),e}()),d=n(1);n(612);window.requestAnimFrame=window.requestAnimationFrame||window.webkitRequestAnimationFrame||window.mozRequestAnimationFrame||function(e){window.setTimeout(e,1e3/60)};var g=function(e){c()(n,e);var t=u()(n);function n(e){var a;return o()(this,n),(a=t.call(this,e)).scrollHandle=function(){(window.pageYOffset||document.documentElement.scrollTop)>400?a.state.top||a.setState({top:!0}):a.state.top&&a.setState({top:!1})},a.onCollapse=function(){a.setState({expand:!1})},a.onExpand=function(){a.setState({expand:!0})},a.onTop=function(){var e=new Date,t=window.pageYOffset||document.documentElement.scrollTop;!function n(){if((window.pageYOffset||document.documentElement.scrollTop)>0){var a=new Date;window.scrollTo(0,t-(a-e)*t/400),window.requestAnimFrame(n)}}()},a.surveyUrl="".concat(d.g.homeServer,"/survey.htm?fromPage=aedetail"),a.state={top:!1,expand:!Object(h.isiPad)(),shares:[{type:"facebook",link:m.getFBUrl()},{type:"pinterest",link:m.getPinterestUrl()},{type:"vk",link:m.getVKUrl()},{type:"twitter",link:m.getTwinnerUrl()}]},a}return r()(n,[{key:"componentDidMount",value:function(){window.addEventListener("scroll",this.scrollHandle)}},{key:"componentWillUnmount",value:function(){window.removeEventListener("scroll",this.scrollHandle)}},{key:"render",value:function(){var e={clk_trigger:"",ae_button_type:"detail-sns-shares",ae_page_type:"detail_page",ae_page_area:"sns_shares",ae_object_type:"share",ae_object_value:"report-bug",st_page_id:window.dmtrack_pageid};return React.createElement("div",{className:"product-fix-sns"},React.createElement("div",{className:"product-fix-wrap"},React.createElement("div",{style:{visibility:this.state.expand?"visible":"hidden"}},React.createElement("div",{className:"sns-shares"},this.state.shares.map((function(t){var n=Object.assign({},e,{ae_object_value:t.type});return React.createElement("span",Object.assign({"data-spm":"share",className:"ui-fixed-panel-unit ui-fixed-panel-".concat(t.type)},n),React.createElement("a",{href:t.link,target:"_blank"}))}))),React.createElement("div",{className:"sns-secondary"},React.createElement("span",Object.assign({className:"ui-fixed-panel-unit ui-fixed-panel-survey"},e),React.createElement("a",{href:this.surveyUrl,target:"_blank"})))),this.state.expand?React.createElement("span",{className:"ui-fixed-panel-unit ui-fixed-panel-collapse"},React.createElement("a",{onClick:this.onCollapse})):React.createElement("span",{className:"ui-fixed-panel-unit ui-fixed-panel-expand"},React.createElement("a",{onClick:this.onExpand})),React.createElement("span",{className:"ui-fixed-panel-unit ui-fixed-panel-go-top",style:{visibility:this.state.top?"visible":"hidden"}},React.createElement("a",{onClick:this.onTop}))))}}]),n}(p.Component);t.default=g}}]);