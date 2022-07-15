clear; clc;
% 用多元线性回归进行复杂形式的函数拟合

x = linspace(-1, 1, 21); x = x';    % 数据点
rng(1);
% 生成带高斯噪音的数据
A = 0.5;  % 噪音幅度
yData = 3.*varphi_1(x) + 4.*varphi_2(x) ...
        + 5.*varphi_3(x) + 7.*varphi_4(x) + 6 ...
        + A.*randn(size(x));

% 将各个基函数作为一个元
x1 = varphi_1(x); x2 = varphi_2(x);
x3 = varphi_3(x); x4 = varphi_4(x);
xData = [x1, x2, x3, x4];
% 理论上：w = [3, 4, 5, 7], b = 6
% yData = xData * (w') + b;

eta = 0.1; % 学习率
tol = 1e-5; % 两次迭代代价函数之差 < tol，则迭代终止
maxIterationNum = 100000;    % 最大迭代次数
w0 = rand(size(xData(1, :)));
b0 = rand(1, 1);   % 随机初始化初值
% w0 = [3, 4, 5, 7] + rand(size(xData(1, :)));
% b0 = 6 + rand(1, 1);   % 随机初始化初值
% w0 = [3, 4, 5, 7];
% b0 = 6;   % 随机初始化初值


% 新建模型
model = LinearRegression(eta, tol, maxIterationNum);
% 拟合
model = model.Fit(xData, yData, w0, b0);
fprintf("拟合结果：\n");
w = model.w
b = model.b
fprintf("理论上：\n");
w = [3, 4, 5, 7]
b = 6

% 数据可视化
X = linspace(-2, 2, 101); X = X';
X1 = varphi_1(X); X2 = varphi_2(X);
X3 = varphi_3(X); X4 = varphi_4(X);
XData = [X1, X2, X3, X4];

Y_predict = model.Predict(XData);       % 预测值
YData = 3.*varphi_1(X) + 4.*varphi_2(X) ... % 实际值
        + 5.*varphi_3(X) + 7.*varphi_4(X) + 6;

scatter(x, yData, 'filled', 'DisplayName', 'Data points');
hold on;
plot(X, Y_predict, 'DisplayName', 'y_{predict}');
hold on;
plot(X, YData, 'DisplayName', 'yData');

xlabel('x')
ylabel('y')
grid on;
legend();


function y = varphi_1(x)
    y = sin(x);
end

function y = varphi_2(x)
    y = exp(x);
end

function y = varphi_3(x)
    y = x.^2;
end

function y = varphi_4(x)
    y = x.^3;
end


